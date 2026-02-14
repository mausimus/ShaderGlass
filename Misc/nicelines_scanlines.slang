#version 450
// nicelines v7 (2026-02-13)

// === PASS 1: SCANLINES ===
// This pass handles: scanline simulation with energy preservation
// Input: blurred linear-space image from pass 0
// Output: scanline image in linear space (float framebuffer)
//
// Misconvergence: R/B beams offset from G (reference) with increasing
// displacement toward screen edges, simulating real CRT gun misalignment.
// Each channel gets its own independent scanline position and mask.

// === PARAMETERS ===
#pragma parameter SCANLINE_MODE "SCANLINES MODE (224p / 240p / Manual)" 1.0 0.0 2.0 1.0
#pragma parameter MANUAL_LINES "MANUAL Mode: Number of Scanlines" 360.0 120.0 480.0 5.0
#pragma parameter FINETUNE_LINES "FINETUNE Number of Scanlines" 0.0 -5.0 5.0 1.0
#pragma parameter AUTO_LINES "ROUND Number of Scanlines to Nearest Multiple of Resolution" 0.0 0.0 1.0 1.0
#pragma parameter MISCONVERGENCE "MISCONVERGENCE Off/On" 1.0 0.0 1.0 1.0
#pragma parameter CONV_STRENGTH "MISCONVERGENCE Strength" 0.25 0.0 0.35 0.05
#pragma parameter DEBUG_STAGES "DEBUG: Show Processing Stages Side-by-Side" 1.0 0.0 1.0 1.0
#pragma parameter DEBUG_SPLIT "DEBUG: Comparison Split (Off/Top=Input/Bot=Input)" 0.0 0.0 2.0 1.0

layout(std140, set = 0, binding = 0) uniform UBO {
    mat4 MVP;
    vec4 SourceSize;
    vec4 OriginalSize;
    vec4 OutputSize;
    uint FrameCount;
    float SCANLINE_MODE;
    float MANUAL_LINES;
    float FINETUNE_LINES;
    float AUTO_LINES;
    float MISCONVERGENCE;
    float CONV_STRENGTH;
    float DEBUG_STAGES;
    float DEBUG_SPLIT;
} global;

// === VERTEX STAGE ===
#pragma stage vertex
layout(location = 0) in vec4 Position;
layout(location = 1) in vec2 TexCoord;
layout(location = 0) out vec2 vTexCoord;

void main() {
    gl_Position = global.MVP * Position;
    vTexCoord = TexCoord;
}

// === FRAGMENT STAGE ===
#pragma stage fragment
layout(location = 0) in vec2 vTexCoord;
layout(location = 0) out vec4 FragColor;
layout(set = 0, binding = 2) uniform sampler2D Source;  // Pass 0 output (blurred, linear space)

// Calculate target scanline count
float getTargetLines() {
    float targetLines;
    int mode = int(global.SCANLINE_MODE + 0.5);
    if(mode == 0) {
        targetLines = 224.0;
    } else if(mode == 1) {
        targetLines = 240.0;
    } else {
        targetLines = global.MANUAL_LINES;
    }
    targetLines += global.FINETUNE_LINES;

    if(global.AUTO_LINES > 0.5) {
        float outputHeight = global.OutputSize.y;
        float idealPixelsPerLine = outputHeight / targetLines;
        float pixelsPerLine = max(1.0, round(idealPixelsPerLine));
        targetLines = outputHeight / pixelsPerLine;
    }
    return targetLines;
}

// Calculate scanline width for a given color (single-channel version)
float calcScanlineWidth1(float chanValue, float baseWidth, float minWidth, float maxWidth) {
    // Dark path: interpolate minWidth → baseWidth based on channel value
    float t = clamp(chanValue / baseWidth, 0.0, 1.0);
    float darkWidth = mix(minWidth, baseWidth, t);

    // Bright path: width follows channel value
    float brightWidth = clamp(chanValue, baseWidth, maxWidth);

    // Smooth blend between dark and bright paths
    float blendZone = 0.1;
    float blend = smoothstep(baseWidth - blendZone * baseWidth, baseWidth + blendZone * baseWidth, chanValue);

    return mix(darkWidth, brightWidth, blend);
}

// Process one channel through the scanline pipeline at a given UV
// Returns the energy-preserved, scanline-masked value for one channel
float processScanlineChannel(vec2 uv, int channel, float targetLines,
                              float baseWidth, float minWidth, float maxWidth, float edgeSoftness) {
    // Calculate scanline position (CENTER-ANCHORED)
    float distFromCenter = uv.y - 0.5;
    float scanlinePos = distFromCenter * targetLines;
    float scanlineIndex = floor(scanlinePos);

    // Sample at scanline center
    float sampleScanlinePos = scanlineIndex + 0.5;
    float sampleDistFromCenter = sampleScanlinePos / targetLines;
    vec2 sampleUV = vec2(uv.x, clamp(0.5 + sampleDistFromCenter, 0.0, 1.0));

    float withinScanline = fract(scanlinePos);

    // Sample the blurred image (already in linear space from pass 0)
    vec3 texColor = texture(Source, sampleUV).rgb;
    float chanValue = (channel == 0) ? texColor.r : (channel == 1) ? texColor.g : texColor.b;

    // Per-channel width based on channel brightness
    float scanlineWidth = calcScanlineWidth1(chanValue, baseWidth, minWidth, maxWidth);
    // Strict guard: prevent energy concentration from exceeding per-pixel channel max.
    // If input chanValue <= 1.0, this guarantees outputValue <= 1.0.
    scanlineWidth = max(scanlineWidth, chanValue);
    float outputValue = chanValue / scanlineWidth;

    // Scanline mask with soft edges
    float halfGap = (1.0 - scanlineWidth) * 0.5;
    float scanlineMask = smoothstep(halfGap - edgeSoftness, halfGap + edgeSoftness, withinScanline) *
                         smoothstep(1.0 - halfGap + edgeSoftness, 1.0 - halfGap - edgeSoftness, withinScanline);

    return outputValue * scanlineMask;
}

// Calculate misconvergence offset for R or B beam
// Radial-outward: offset direction follows the vector from screen center.
// Automatically symmetrical — opposite corners push in opposite directions.
vec2 calcConvergenceOffset(vec2 uv, float strength, float sign) {
    vec2 fromCenter = uv - 0.5;
    float dist = length(fromCenter);
    // Radial direction (outward from center)
    vec2 direction = (dist > 0.001) ? normalize(fromCenter) : vec2(0.0);
    // Quadratic falloff: corners (~0.7) get ~2× more offset than edge centers (~0.5)
    float magnitude = sign * strength * dist * dist * 0.02;
    return direction * magnitude;
}

void main() {
    float baseWidth = 0.3;   // Hardcoded thickness baseline
    float minWidth = 0.15;   // Hardcoded thickness min (must be ≤ baseWidth)
    float maxWidth = 1.0;    // Hardcoded thickness max
    float edgeSoftness = 0.3;

    // === COMPARISON SPLIT: pass through input unchanged ===
    int splitMode = int(global.DEBUG_SPLIT + 0.5);
    bool showRawHere = false;
    if(splitMode == 1 && vTexCoord.y < 0.5) showRawHere = true;
    if(splitMode == 2 && vTexCoord.y >= 0.5) showRawHere = true;
    if(showRawHere) {
        FragColor = texture(Source, vTexCoord);
        return;
    }

    // Debug mode: left 1/3 shows blur-only passthrough
    if(global.DEBUG_STAGES > 0.5 && vTexCoord.x < (1.0 / 3.0)) {
        FragColor = texture(Source, vTexCoord);
        return;
    }

    float targetLines = getTargetLines();

    // === PER-CHANNEL SCANLINE PROCESSING WITH MISCONVERGENCE ===
    vec2 uvR = vTexCoord;
    vec2 uvG = vTexCoord;
    vec2 uvB = vTexCoord;

    if(global.MISCONVERGENCE > 0.5) {
        // R and B beams offset in opposite directions, G stays centered
        // Scale strength relative to scanline size (normalized to 240p baseline)
        float convStrength = global.CONV_STRENGTH * (240.0 / targetLines);
        uvR += calcConvergenceOffset(vTexCoord, convStrength, 1.0);
        uvB += calcConvergenceOffset(vTexCoord, convStrength, -1.0);
        // Clamp to valid UV range
        uvR = clamp(uvR, vec2(0.0), vec2(1.0));
        uvB = clamp(uvB, vec2(0.0), vec2(1.0));
    }

    // Each channel gets its own scanline position (independent beam)
    // TODO: when MISCONVERGENCE is off, all 3 UVs are identical — could optimize
    // to a single texture fetch instead of 3. Negligible perf impact for now.
    float finalR = processScanlineChannel(uvR, 0, targetLines, baseWidth, minWidth, maxWidth, edgeSoftness);
    float finalG = processScanlineChannel(uvG, 1, targetLines, baseWidth, minWidth, maxWidth, edgeSoftness);
    float finalB = processScanlineChannel(uvB, 2, targetLines, baseWidth, minWidth, maxWidth, edgeSoftness);

    // Output in linear space (float framebuffer)
    FragColor = vec4(finalR, finalG, finalB, 1.0);
}
