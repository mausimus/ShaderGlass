#version 450
// nicelines v5 (2026-02-13)

// === PASS 0: DOWNSCALE BLUR ===
// This pass handles: downscale blur prepass and horizontal blur
// Output: blurred image in linear space (float framebuffer)
//
// NOTE: Currently uses box 9-tap blur. Consider replacing with a proper
// downsampling approach (mip-based or bilinear area sampling) in the future.

// === PARAMETERS ===
#pragma parameter BLUR_OFFSET "DOWNSCALING Prepass (Lower = Softer)" 0.0 -0.04 0.12 0.01
#pragma parameter H_BLUR "HORIZONTAL BLUR (worn out tube look)" 1.0 0.0 8.0 0.1
#pragma parameter SCANLINE_MODE "SCANLINES MODE (224p / 240p / Manual)" 1.0 0.0 2.0 1.0
#pragma parameter MANUAL_LINES "MANUAL Mode: Number of Scanlines" 360.0 120.0 480.0 5.0
#pragma parameter FINETUNE_LINES "FINETUNE Number of Scanlines" 0.0 -5.0 5.0 1.0
#pragma parameter AUTO_LINES "ROUND Number of Scanlines to Nearest Multiple of Resolution" 0.0 0.0 1.0 1.0

layout(std140, set = 0, binding = 0) uniform UBO {
    mat4 MVP;
    vec4 SourceSize;
    vec4 OriginalSize;
    vec4 OutputSize;
    uint FrameCount;
    float BLUR_OFFSET;
    float H_BLUR;
    float SCANLINE_MODE;
    float MANUAL_LINES;
    float FINETUNE_LINES;
    float AUTO_LINES;
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
layout(set = 0, binding = 2) uniform sampler2D Source;

// sRGB to linear conversion (input is sRGB from the game)
float SrgbToLinear(float c) {
    return c <= 0.04045 ? c / 12.92 : pow((c + 0.055) / 1.055, 2.4);
}

vec3 SrgbToLinear3(vec3 c) {
    return vec3(SrgbToLinear(c.r), SrgbToLinear(c.g), SrgbToLinear(c.b));
}

// Calculate target scanline count (needed for blur radius)
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

// 9-tap box filter blur
vec3 boxBlur(vec2 uv, vec2 radius) {
    vec3 sum = vec3(0.0);
    sum += SrgbToLinear3(texture(Source, uv + vec2(-radius.x, -radius.y)).rgb);
    sum += SrgbToLinear3(texture(Source, uv + vec2(      0.0, -radius.y)).rgb);
    sum += SrgbToLinear3(texture(Source, uv + vec2( radius.x, -radius.y)).rgb);
    sum += SrgbToLinear3(texture(Source, uv + vec2(-radius.x,       0.0)).rgb);
    sum += SrgbToLinear3(texture(Source, uv).rgb);
    sum += SrgbToLinear3(texture(Source, uv + vec2( radius.x,       0.0)).rgb);
    sum += SrgbToLinear3(texture(Source, uv + vec2(-radius.x,  radius.y)).rgb);
    sum += SrgbToLinear3(texture(Source, uv + vec2(      0.0,  radius.y)).rgb);
    sum += SrgbToLinear3(texture(Source, uv + vec2( radius.x,  radius.y)).rgb);
    return sum / 9.0;
}

// Sample source with downscale blur (box 9-tap)
vec3 sampleWithBlur(vec2 uv, float effectiveScale) {
    if(effectiveScale < 0.999) {
        float blurRadius = (1.0 / effectiveScale - 1.0) * 0.5;
        vec2 texelSize = 1.0 / global.SourceSize.xy;
        vec2 radius = texelSize * blurRadius;

        return boxBlur(uv, radius);
    } else {
        return SrgbToLinear3(texture(Source, uv).rgb);
    }
}

// Sample with horizontal blur (5-tap tent filter, operates in linear space)
vec3 sampleWithHBlur(vec2 uv, float effectiveScale, float hBlur) {
    if(hBlur > 0.001) {
        float pixelSize = 1.0 / global.OutputSize.x;
        float blurRadius = hBlur * pixelSize;

        vec3 cC  = sampleWithBlur(uv, effectiveScale);
        vec3 cL1 = sampleWithBlur(vec2(uv.x - blurRadius * 0.5, uv.y), effectiveScale);
        vec3 cL2 = sampleWithBlur(vec2(uv.x - blurRadius, uv.y), effectiveScale);
        vec3 cR1 = sampleWithBlur(vec2(uv.x + blurRadius * 0.5, uv.y), effectiveScale);
        vec3 cR2 = sampleWithBlur(vec2(uv.x + blurRadius, uv.y), effectiveScale);

        return cL2 * 0.1 + cL1 * 0.2 + cC * 0.4 + cR1 * 0.2 + cR2 * 0.1;
    } else {
        return sampleWithBlur(uv, effectiveScale);
    }
}

void main() {
    float targetLines = getTargetLines();
    float baselineScale = targetLines / global.SourceSize.y;
    float effectiveScale = clamp(baselineScale + global.BLUR_OFFSET, 0.02, 1.0);

    // Apply downscale + horizontal blur, output in linear space
    vec3 color = sampleWithHBlur(vTexCoord, effectiveScale, global.H_BLUR);

    FragColor = vec4(color, 1.0);
}
