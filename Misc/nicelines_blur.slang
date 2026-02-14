#version 450
// nicelines v17 (2026-02-14)

// === PASS 0: DOWNSCALE BLUR ===
// This pass handles: downscale blur prepass and horizontal blur
// Output: blurred image in linear space (float framebuffer)

// === PARAMETERS ===
#pragma parameter BLUR_OFFSET "BEAM FOCUS" 0.05 -0.04 0.12 0.01
#pragma parameter H_BLUR "HORIZONTAL SMEAR" 1.0 0.0 8.0 0.1
#pragma parameter SCANLINE_MODE "180p / 224p / 240p / 270p / 360p / MANUAL" 2.0 0.0 5.0 1.0
#pragma parameter MANUAL_LINES "MANUAL Scanline Count" 360.0 120.0 480.0 5.0
#pragma parameter AUTO_LINES "SCANLINE ROUNDING (for 3D; anti-moire)" 0.0 0.0 1.0 1.0
#pragma parameter COMPARE "COMPARE (Off / Left / Right)" 0.0 0.0 2.0 1.0
#pragma parameter DOT_CRAWL "DOT CRAWL (Off / On)" 0.0 0.0 1.0 1.0
#pragma parameter DOT_CRAWL_INTENSITY "DOT CRAWL Intensity" 0.25 0.0 1.0 0.01
#pragma parameter DOT_CRAWL_DEBUG "DOT CRAWL Debug (Off / Pre / Diff)" 0.0 0.0 2.0 1.0

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
    float AUTO_LINES;
    float COMPARE;
    float DOT_CRAWL;
    float DOT_CRAWL_INTENSITY;
    float DOT_CRAWL_DEBUG;
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

// Sample source with area-average downscale
vec3 sampleWithBlur(vec2 uv, float effectiveScale) {
    if(effectiveScale >= 0.999) {
        return SrgbToLinear3(texture(Source, uv).rgb);
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

vec3 applyDotCrawl(vec3 linearColor, vec2 uv) {
    if(global.DOT_CRAWL < 0.5) {
        return linearColor;
    }

    float y = dot(linearColor, vec3(0.299, 0.587, 0.114));
    float u = linearColor.b - y;
    float v = linearColor.r - y;

    float chroma = length(vec2(u, v));
    float chromaGate = smoothstep(0.015, 0.20, chroma);

    vec2 px = uv * global.OutputSize.xy;
    float subcarrier = px.x * 2.15 + px.y * 1.57 + float(global.FrameCount) * 0.85;
    float crawl = sin(subcarrier) * 0.75 + sin(subcarrier * 0.47 + 1.13) * 0.25;

    float amount = 0.08 * clamp(global.DOT_CRAWL_INTENSITY, 0.0, 1.0) * chromaGate;
    u += crawl * amount;
    v -= crawl * amount * 0.85;

    float r = y + v;
    float b = y + u;
    float g = (y - 0.299 * r - 0.114 * b) / 0.587;

    return clamp(vec3(r, g, b), 0.0, 1.0);
}

void main() {
    int compareMode = int(global.COMPARE + 0.5);
    bool showRawHere = false;
    if(compareMode == 1 && vTexCoord.x < 0.5) showRawHere = true;
    if(compareMode == 2 && vTexCoord.x >= 0.5) showRawHere = true;
    if(showRawHere) {
        FragColor = vec4(SrgbToLinear3(texture(Source, vTexCoord).rgb), 1.0);
        return;
    }

    float targetLines = getTargetLines();
    float baselineScale = targetLines / global.SourceSize.y;
    float effectiveScale = clamp(baselineScale + global.BLUR_OFFSET, 0.02, 1.0);

    // Apply downscale + horizontal blur, output in linear space
    vec3 baseColor = sampleWithHBlur(vTexCoord, effectiveScale, global.H_BLUR);
    vec3 crawledColor = applyDotCrawl(baseColor, vTexCoord);

    int crawlDebug = int(global.DOT_CRAWL_DEBUG + 0.5);
    vec3 color;
    if(crawlDebug == 1) {
        // Show pre-pass output with crawl applied.
        color = crawledColor;
    } else if(crawlDebug == 2) {
        // Difference view (boosted) to reveal subtle crawl signal.
        color = clamp(abs(crawledColor - baseColor) * 20.0, 0.0, 1.0);
    } else {
        color = crawledColor;
    }

    FragColor = vec4(color, 1.0);
}
