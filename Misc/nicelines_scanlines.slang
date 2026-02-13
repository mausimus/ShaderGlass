#version 450
// nicelines v5 (2026-02-13)

// === PASS 1: SCANLINES ===
// This pass handles: scanline simulation with energy preservation
// Input: blurred linear-space image from pass 0
// Output: scanline image in linear space (float framebuffer)
//
// TODO: add misconvergence effect (RGB channel offset) to this pass

// === PARAMETERS ===
#pragma parameter SCANLINE_MODE "SCANLINES MODE (224p / 240p / Manual)" 1.0 0.0 2.0 1.0
#pragma parameter MANUAL_LINES "MANUAL Mode: Number of Scanlines" 360.0 120.0 480.0 5.0
#pragma parameter FINETUNE_LINES "FINETUNE Number of Scanlines" 0.0 -5.0 5.0 1.0
#pragma parameter AUTO_LINES "ROUND Number of Scanlines to Nearest Multiple of Resolution" 0.0 0.0 1.0 1.0
#pragma parameter DEBUG_STAGES "DEBUG: Show Processing Stages Side-by-Side" 1.0 0.0 1.0 1.0

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
    float DEBUG_STAGES;
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

// Calculate scanline width for a given color
float calcScanlineWidth(vec3 color, float baseWidth, float minWidth, float maxWidth) {
    float maxColorChannel = max(max(color.r, color.g), color.b);
    float luma = dot(color, vec3(0.2126, 0.7152, 0.0722));

    // Dark path: interpolate minWidth → baseWidth based on luma
    float t = clamp(luma / baseWidth, 0.0, 1.0);
    float darkWidth = mix(minWidth, baseWidth, t);

    // Bright path: width follows max channel
    float brightWidth = clamp(maxColorChannel, baseWidth, maxWidth);

    // Smooth blend between dark and bright paths around baseWidth threshold
    float blendZone = 0.1;
    float blend = smoothstep(baseWidth - blendZone * baseWidth, baseWidth + blendZone * baseWidth, maxColorChannel);

    return mix(darkWidth, brightWidth, blend);
}

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

void main() {
    float baseWidth = 0.3;   // Hardcoded thickness baseline
    float minWidth = 0.15;   // Hardcoded thickness min (must be ≤ baseWidth)
    float maxWidth = 1.0;    // Hardcoded thickness max

    // Debug mode: left 1/3 shows blur-only passthrough
    if(global.DEBUG_STAGES > 0.5 && vTexCoord.x < (1.0 / 3.0)) {
        FragColor = texture(Source, vTexCoord);
        return;
    }

    float targetLines = getTargetLines();

    // Edge softness
    float edgeSoftness = 0.3;

    // Calculate scanline position (CENTER-ANCHORED)
    float distFromCenter = vTexCoord.y - 0.5;
    float scanlinePos = distFromCenter * targetLines;
    float scanlineIndex = floor(scanlinePos);

    // Sample at scanline center
    float sampleScanlinePos = scanlineIndex + 0.5;
    float sampleDistFromCenter = sampleScanlinePos / targetLines;
    vec2 sampleUV = vec2(vTexCoord.x, clamp(0.5 + sampleDistFromCenter, 0.0, 1.0));

    float withinScanline = fract(scanlinePos);

    // Sample the blurred image (already in linear space from pass 0)
    vec3 color = texture(Source, sampleUV).rgb;

    // Calculate scanline width and energy-preserved color (per-channel, no clamp)
    float scanlineWidth = calcScanlineWidth(color, baseWidth, minWidth, maxWidth);
    vec3 outputColor = color / scanlineWidth;

    // Scanline mask with soft edges
    float halfGap = (1.0 - scanlineWidth) * 0.5;
    float scanlineMask = smoothstep(halfGap - edgeSoftness, halfGap + edgeSoftness, withinScanline) *
                         smoothstep(1.0 - halfGap + edgeSoftness, 1.0 - halfGap - edgeSoftness, withinScanline);

    vec3 finalColor = outputColor * scanlineMask;

    // Output in linear space (float framebuffer)
    FragColor = vec4(finalColor, 1.0);
}
