#version 450
// nicelines v17 (2026-02-14)

// === PASS 0: DOWNSCALE BLUR ===
// This pass handles: downscale blur prepass and horizontal blur
// Output: blurred image in linear space (float framebuffer)
//
// Downsample options:
//   0 = Area (current)
//   1 = Stable Box (texel-center integrated)
//   2 = Bicubic (catmull-rom)
//   3 = Gaussian prefilter

// === PARAMETERS ===
#pragma parameter DOWNSAMPLE_MODE "DOWNSAMPLE Mode (Area / StableBox / Bicubic / Gaussian)" 2.0 0.0 3.0 1.0
#pragma parameter BLUR_OFFSET "DOWNSCALING Prepass (Lower = Softer)" 0.05 -0.04 0.12 0.01
#pragma parameter H_BLUR "HORIZONTAL BLUR (worn out tube look)" 1.0 0.0 8.0 0.1
#pragma parameter SCANLINE_MODE "SCANLINES MODE (180p / 224p / 240p / 270p / 360p / Manual)" 2.0 0.0 5.0 1.0
#pragma parameter MANUAL_LINES "MANUAL Mode: Number of Scanlines" 360.0 120.0 480.0 5.0
#pragma parameter AUTO_LINES "ROUND Number of Scanlines to Nearest Multiple of Resolution" 0.0 0.0 1.0 1.0

layout(std140, set = 0, binding = 0) uniform UBO {
    mat4 MVP;
    vec4 SourceSize;
    vec4 OriginalSize;
    vec4 OutputSize;
    uint FrameCount;
    float DOWNSAMPLE_MODE;
    float BLUR_OFFSET;
    float H_BLUR;
    float SCANLINE_MODE;
    float MANUAL_LINES;
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
        targetLines = 180.0;
    } else if(mode == 1) {
        targetLines = 224.0;
    } else if(mode == 2) {
        targetLines = 240.0;
    } else if(mode == 3) {
        targetLines = 270.0;
    } else if(mode == 4) {
        targetLines = 360.0;
    } else {
        targetLines = global.MANUAL_LINES;
    }
    if(global.AUTO_LINES > 0.5) {
        float outputHeight = global.OutputSize.y;
        float idealPixelsPerLine = outputHeight / targetLines;
        float pixelsPerLine = max(1.0, round(idealPixelsPerLine));
        targetLines = outputHeight / pixelsPerLine;
    }
    return targetLines;
}

// Area-average downsample: loops over all source texels covered by the output pixel
vec3 areaAverage(vec2 uv, float effectiveScale) {
    vec2 texelSize = 1.0 / global.SourceSize.xy;
    // How many source texels per output pixel
    float coverage = 1.0 / effectiveScale;
    // Half-width of the footprint in UV space
    vec2 halfExtent = texelSize * coverage * 0.5;

    // Number of samples per axis (at least 1, cap at 8 for performance)
    int samplesX = clamp(int(ceil(coverage)), 1, 8);
    int samplesY = clamp(int(ceil(coverage)), 1, 8);

    vec3 sum = vec3(0.0);
    float count = 0.0;

    for(int sy = 0; sy < samplesY; sy++) {
        for(int sx = 0; sx < samplesX; sx++) {
            vec2 offset = vec2(
                mix(-halfExtent.x, halfExtent.x, (float(sx) + 0.5) / float(samplesX)),
                mix(-halfExtent.y, halfExtent.y, (float(sy) + 0.5) / float(samplesY))
            );
            vec2 sampleUV = clamp(uv + offset, vec2(0.0), vec2(1.0));
            sum += SrgbToLinear3(texture(Source, sampleUV).rgb);
            count += 1.0;
        }
    }
    return sum / count;
}

// Stable box downsample: integrates over texel centers in source pixel space.
// More temporally stable under subpixel motion than sparse pattern samples.
vec3 stableBoxAverage(vec2 uv, float effectiveScale) {
    vec2 srcSize = global.SourceSize.xy;
    vec2 uvMin = clamp(uv - vec2(0.5 / effectiveScale) / srcSize, vec2(0.0), vec2(1.0));
    vec2 uvMax = clamp(uv + vec2(0.5 / effectiveScale) / srcSize, vec2(0.0), vec2(1.0));

    // Convert to source texel index range (center-based)
    int x0 = int(floor(uvMin.x * srcSize.x));
    int x1 = int(ceil (uvMax.x * srcSize.x));
    int y0 = int(floor(uvMin.y * srcSize.y));
    int y1 = int(ceil (uvMax.y * srcSize.y));

    // Cap bounds for performance
    x0 = clamp(x0, 0, int(srcSize.x) - 1);
    y0 = clamp(y0, 0, int(srcSize.y) - 1);
    x1 = clamp(x1, 0, int(srcSize.x) - 1);
    y1 = clamp(y1, 0, int(srcSize.y) - 1);

    vec3 sum = vec3(0.0);
    float count = 0.0;

    for(int y = y0; y <= y1; y++) {
        for(int x = x0; x <= x1; x++) {
            vec2 sampleUV = (vec2(float(x), float(y)) + 0.5) / srcSize;
            sum += SrgbToLinear3(texture(Source, sampleUV).rgb);
            count += 1.0;
        }
    }

    if(count <= 0.0) {
        return SrgbToLinear3(texture(Source, uv).rgb);
    }
    return sum / count;
}

float cubicWeight(float x) {
    float a = -0.5; // Catmull-Rom
    x = abs(x);
    if(x <= 1.0) {
        return (a + 2.0) * x * x * x - (a + 3.0) * x * x + 1.0;
    } else if(x < 2.0) {
        return a * x * x * x - 5.0 * a * x * x + 8.0 * a * x - 4.0 * a;
    }
    return 0.0;
}

vec3 bicubicSample(vec2 uv) {
    vec2 size = global.SourceSize.xy;
    vec2 p = uv * size - 0.5;
    vec2 base = floor(p);
    vec2 f = fract(p);

    vec3 sum = vec3(0.0);
    float sumW = 0.0;

    for(int j = -1; j <= 2; j++) {
        for(int i = -1; i <= 2; i++) {
            float wx = cubicWeight(float(i) - f.x);
            float wy = cubicWeight(float(j) - f.y);
            float w = wx * wy;

            vec2 tap = (base + vec2(float(i), float(j)) + 0.5) / size;
            tap = clamp(tap, vec2(0.0), vec2(1.0));
            sum += SrgbToLinear3(texture(Source, tap).rgb) * w;
            sumW += w;
        }
    }

    if(sumW <= 0.0) {
        return SrgbToLinear3(texture(Source, uv).rgb);
    }
    return sum / sumW;
}

vec3 gaussianPrefilter(vec2 uv, float effectiveScale) {
    vec2 texel = 1.0 / global.SourceSize.xy;
    float radiusPx = max(0.35, (1.0 / effectiveScale - 1.0) * 0.65);
    float sigma = max(0.5, radiusPx * 0.75);

    vec3 sum = vec3(0.0);
    float sumW = 0.0;

    for(int y = -2; y <= 2; y++) {
        for(int x = -2; x <= 2; x++) {
            vec2 o = vec2(float(x), float(y));
            float d2 = dot(o, o);
            float w = exp(-d2 / (2.0 * sigma * sigma));
            vec2 tap = clamp(uv + o * radiusPx * texel, vec2(0.0), vec2(1.0));
            sum += SrgbToLinear3(texture(Source, tap).rgb) * w;
            sumW += w;
        }
    }

    return sum / sumW;
}

// Sample source with area-average downscale
vec3 sampleWithBlur(vec2 uv, float effectiveScale) {
    if(effectiveScale >= 0.999) {
        return SrgbToLinear3(texture(Source, uv).rgb);
    }

    int mode = int(global.DOWNSAMPLE_MODE + 0.5);
    if(mode == 3) {
        return gaussianPrefilter(uv, effectiveScale);
    } else if(mode == 2) {
        return bicubicSample(uv);
    } else if(mode == 1) {
        return stableBoxAverage(uv, effectiveScale);
    }
    return areaAverage(uv, effectiveScale);
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
