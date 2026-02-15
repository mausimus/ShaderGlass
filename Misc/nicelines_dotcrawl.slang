#version 450
// nicelines v7 dot crawl pass (2026-02-14)

// === PASS 1: DOT CRAWL (v7) ===
// Input: blurred linear-space image from pass 0
// Output: dot-crawl processed linear image for scanlines pass

#pragma parameter DOT_CRAWL "DOT CRAWL v7 (Off / On)" 1.0 0.0 1.0 1.0
#pragma parameter DOT_CRAWL_INTENSITY "DOT CRAWL v7 Intensity" 3.0 0.0 6.0 0.01
#pragma parameter V7_EDGE_GROW "DOT CRAWL v7 Edge Grow" 12.0 0.0 24.0 1.0
#pragma parameter V7_LINE_THICKNESS "DOT CRAWL v7 Line Thickness" 0.65 0.1 1.0 0.05
#pragma parameter V7_SWAP_SIZE "DOT CRAWL v7 Swap Size" 1.8 0.0 8.0 0.1
#pragma parameter V7_SPEED "DOT CRAWL v7 Horizontal Speed" 1.1 0.0 3.0 0.05
#pragma parameter V7_COLOR_BIAS "DOT CRAWL v7 Color Select" 2.0 0.2 4.0 0.05
#pragma parameter DOT_CRAWL_DEBUG "DOT CRAWL Debug (Off / Bypass / Diff)" 0.0 0.0 2.0 1.0

#pragma parameter SCANLINE_MODE "180p / 224p / 240p / 270p / 360p / MANUAL" 2.0 0.0 5.0 1.0
#pragma parameter MANUAL_LINES "MANUAL Scanline Count" 360.0 120.0 480.0 5.0
#pragma parameter AUTO_LINES "SCANLINE ROUNDING (for 3D; anti-moire)" 0.0 0.0 1.0 1.0

layout(std140, set = 0, binding = 0) uniform UBO {
    mat4 MVP;
    vec4 SourceSize;
    vec4 OriginalSize;
    vec4 OutputSize;
    uint FrameCount;
    float DOT_CRAWL;
    float DOT_CRAWL_INTENSITY;
    float V7_EDGE_GROW;
    float V7_LINE_THICKNESS;
    float V7_SWAP_SIZE;
    float V7_SPEED;
    float V7_COLOR_BIAS;
    float DOT_CRAWL_DEBUG;
    float SCANLINE_MODE;
    float MANUAL_LINES;
    float AUTO_LINES;
} global;

#pragma stage vertex
layout(location = 0) in vec4 Position;
layout(location = 1) in vec2 TexCoord;
layout(location = 0) out vec2 vTexCoord;

void main() {
    gl_Position = global.MVP * Position;
    vTexCoord = TexCoord;
}

#pragma stage fragment
layout(location = 0) in vec2 vTexCoord;
layout(location = 0) out vec4 FragColor;
layout(set = 0, binding = 2) uniform sampler2D Source;

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

float coolScore(vec3 c) {
    float cool = (c.b + c.g * 0.55) - c.r * 0.75;
    return smoothstep(0.02, 0.25, cool);
}

float warmScore(vec3 c) {
    float warm = (c.r + c.g * 0.45) - c.b * 0.85;
    return smoothstep(0.02, 0.25, warm);
}

float transitionAt(vec2 uv, vec2 texel) {
    const vec3 LUMA = vec3(0.299, 0.587, 0.114);

    vec3 cL = texture(Source, clamp(uv - vec2(texel.x, 0.0), vec2(0.0), vec2(1.0))).rgb;
    vec3 cR = texture(Source, clamp(uv + vec2(texel.x, 0.0), vec2(0.0), vec2(1.0))).rgb;

    float coolL = coolScore(cL);
    float warmL = warmScore(cL);
    float coolR = coolScore(cR);
    float warmR = warmScore(cR);

    float groupTransition = max(coolL * warmR, warmL * coolR);

    vec3 chL = cL - vec3(dot(cL, LUMA));
    vec3 chR = cR - vec3(dot(cR, LUMA));
    float chromaContrast = length(chR - chL);

    float colorBias = clamp(global.V7_COLOR_BIAS, 0.2, 4.0);
    float edge = smoothstep(0.015, 0.13, chromaContrast * colorBias);
    return groupTransition * edge;
}

float transitionGrown(vec2 uv, vec2 texel) {
    int grow = clamp(int(floor(global.V7_EDGE_GROW + 0.5)), 0, 24);
    float best = 0.0;
    for(int i = -24; i <= 24; i++) {
        if(abs(i) <= grow) {
            float t = transitionAt(uv + vec2(float(i) * texel.x, 0.0), texel);
            best = max(best, t);
        }
    }
    return best;
}

vec3 applyDotCrawlV7(vec3 linearColor, vec2 uv) {
    vec2 srcTexel = 1.0 / global.SourceSize.xy;
    float edgeMask = transitionGrown(uv, srcTexel);

    float targetLines = getTargetLines();
    float scanlinePx = max(1.0, global.OutputSize.y / targetLines);
    float lineThicknessPx = max(0.5, global.V7_LINE_THICKNESS * scanlinePx);

    vec2 px = uv * global.OutputSize.xy;
    float travelPx = float(global.FrameCount) * global.V7_SPEED;

    // NE->SW diagonal lines: constant (x + y)
    float lineCoord = (px.x + travelPx + px.y) / lineThicknessPx;
    float lineId = floor(lineCoord);

    // Two-line swap grouping with alternating pair direction
    float pairId = floor(lineId / 2.0);
    float lineInPair = mod(lineId, 2.0);
    float pairSign = (mod(pairId, 2.0) < 1.0) ? -1.0 : 1.0;
    float intraSign = (lineInPair < 1.0) ? 1.0 : -1.0;
    float swapSign = pairSign * intraSign;

    vec2 swapDir = normalize(vec2(1.0, 1.0)); // NW -> SE
    float swapPx = global.V7_SWAP_SIZE * clamp(global.DOT_CRAWL_INTENSITY, 0.0, 6.0) * edgeMask;
    vec2 swapUV = uv + swapDir * (swapSign * swapPx) * srcTexel;
    vec3 swapped = texture(Source, clamp(swapUV, vec2(0.0), vec2(1.0))).rgb;

    // Chroma-only masking
    float y0 = dot(linearColor, vec3(0.299, 0.587, 0.114));
    float y1 = dot(swapped,     vec3(0.299, 0.587, 0.114));
    vec3 ch1 = swapped - vec3(y1);
    vec3 outColor = clamp(vec3(y0) + ch1, 0.0, 1.0);

    return mix(linearColor, outColor, clamp(edgeMask, 0.0, 1.0));
}

void main() {
    vec3 baseColor = texture(Source, vTexCoord).rgb;

    vec3 crawledColor = (global.DOT_CRAWL > 0.5)
        ? applyDotCrawlV7(baseColor, vTexCoord)
        : baseColor;

    int crawlDebug = int(global.DOT_CRAWL_DEBUG + 0.5);
    vec3 color;
    if(crawlDebug == 1) {
        color = baseColor;
    } else if(crawlDebug == 2) {
        color = clamp(abs(crawledColor - baseColor) * 35.0, 0.0, 1.0);
    } else {
        color = crawledColor;
    }

    FragColor = vec4(color, 1.0);
}
