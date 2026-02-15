#version 450
// nicelines v7 dot crawl pass (2026-02-14)

// === PASS 1: DOT CRAWL (v7) ===
// Input: blurred linear-space image from pass 0
// Output: dot-crawl processed linear image for scanlines pass

#pragma parameter DOT_CRAWL "DOT CRAWL v7 (Off / On)" 1.0 0.0 1.0 1.0
#pragma parameter DOT_CRAWL_INTENSITY "DOT CRAWL v7 Intensity" 6.0 0.0 6.0 0.01
#pragma parameter V7_EDGE_GROW "DOT CRAWL v7 Edge Grow" 5.0 0.0 24.0 1.0
#pragma parameter V7_LINE_THICKNESS "DOT CRAWL v7 Line Thickness" 0.4 0.1 1.0 0.05
#pragma parameter V7_SWAP_SIZE "DOT CRAWL v7 Swap Size" 1.0 0.0 8.0 0.1
#pragma parameter V7_SPEED "DOT CRAWL v7 Horizontal Speed" 1.1 0.0 3.0 0.05
#pragma parameter V7_COLOR_BIAS "DOT CRAWL v7 Color Select" 2.0 0.2 4.0 0.05
#pragma parameter V7_MASK_MODE "DOT CRAWL Mask Mode (Color / Vertical Red)" 0.0 0.0 1.0 1.0
#pragma parameter V7_MASK_SAMPLE_SPAN "DOT CRAWL Mask Sample Span (scanlines)" 2.5 0.5 6.0 0.1
#pragma parameter V7_MASK_GROW_MODE "DOT CRAWL Mask Grow Mode (Max / Avg / Falloff)" 0.0 0.0 2.0 1.0
#pragma parameter V7_MASK_CENTER_SAT "DOT CRAWL Mask Center Sat Clamp" 0.10 0.0 0.6 0.01
#pragma parameter V7_MASK_SAT_LOW "DOT CRAWL Mask Sat Low" 0.08 0.0 0.6 0.01
#pragma parameter V7_MASK_SAT_HIGH "DOT CRAWL Mask Sat High" 0.32 0.0 0.8 0.01
#pragma parameter V7_MASK_NEUTRAL_REJECT "DOT CRAWL Mask Neutral Reject" 0.10 0.0 0.6 0.01
#pragma parameter V7_VRED_SEGMENT_X "DOT CRAWL V-Red Segment X/n" 1.0 0.25 4.0 0.05
#pragma parameter V7_VRED_DELTA "DOT CRAWL V-Red Delta Threshold" 0.18 0.01 1.0 0.01
#pragma parameter V7_VRED_SLIVER_EXTRA "DOT CRAWL V-Red Sliver Extra" 0.0 0.0 3.0 0.05
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
    float V7_MASK_MODE;
    float V7_MASK_SAMPLE_SPAN;
    float V7_MASK_GROW_MODE;
    float V7_MASK_CENTER_SAT;
    float V7_MASK_SAT_LOW;
    float V7_MASK_SAT_HIGH;
    float V7_MASK_NEUTRAL_REJECT;
    float V7_VRED_SEGMENT_X;
    float V7_VRED_DELTA;
    float V7_VRED_SLIVER_EXTRA;
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

float satAmount(vec3 c) {
    return max(c.r, max(c.g, c.b)) - min(c.r, min(c.g, c.b));
}

float rednessRatio(vec3 c) {
    return c.r / max(c.g + c.b, 0.0001);
}

float transitionAt(vec2 uv, float sampleStepX) {
    const vec3 LUMA = vec3(0.299, 0.587, 0.114);

    vec3 cL = texture(Source, clamp(uv - vec2(sampleStepX, 0.0), vec2(0.0), vec2(1.0))).rgb;
    vec3 cR = texture(Source, clamp(uv + vec2(sampleStepX, 0.0), vec2(0.0), vec2(1.0))).rgb;
    vec3 cC = texture(Source, clamp(uv, vec2(0.0), vec2(1.0))).rgb;

    float coolL = coolScore(cL);
    float warmL = warmScore(cL);
    float coolR = coolScore(cR);
    float warmR = warmScore(cR);

    float groupTransition = max(coolL * warmR, warmL * coolR);

    vec3 chL = cL - vec3(dot(cL, LUMA));
    vec3 chR = cR - vec3(dot(cR, LUMA));
    float chromaContrast = length(chR - chL);

    float satL = satAmount(cL);
    float satR = satAmount(cR);
    float satC = satAmount(cC);
    float satLow = min(global.V7_MASK_SAT_LOW, global.V7_MASK_SAT_HIGH);
    float satHigh = max(global.V7_MASK_SAT_LOW, global.V7_MASK_SAT_HIGH);
    float saturationGate = smoothstep(satLow, satHigh, min(satL, satR));
    float centerGate = smoothstep(global.V7_MASK_CENTER_SAT, global.V7_MASK_CENTER_SAT + 0.08, satC);

    float colorBias = clamp(global.V7_COLOR_BIAS, 0.2, 4.0);
    float edge = smoothstep(0.015, 0.13, chromaContrast * colorBias);
    float centerChroma = length(cC - vec3(dot(cC, LUMA)));
    float neutralGate = smoothstep(global.V7_MASK_NEUTRAL_REJECT, global.V7_MASK_NEUTRAL_REJECT + 0.10, centerChroma);
    return groupTransition * edge * saturationGate * centerGate * neutralGate;
}

float transitionGrown(vec2 uv, vec2 texel, float sampleStepX) {
    int grow = clamp(int(floor(global.V7_EDGE_GROW + 0.5)), 0, 24);
    int growMode = clamp(int(floor(global.V7_MASK_GROW_MODE + 0.5)), 0, 2);
    float best = 0.0;
    float sum = 0.0;
    float weighted = 0.0;
    float weightSum = 0.0;
    float count = 0.0;
    for(int i = -24; i <= 24; i++) {
        if(abs(i) <= grow) {
            float t = transitionAt(uv + vec2(float(i) * texel.x, 0.0), sampleStepX);
            best = max(best, t);
            sum += t;
            count += 1.0;
            float w = max(0.0, 1.0 - (float(abs(i)) / float(grow + 1)));
            weighted += t * w;
            weightSum += w;
        }
    }

    if(growMode == 1) {
        return sum / max(count, 1.0);
    }
    if(growMode == 2) {
        return weighted / max(weightSum, 0.0001);
    }
    return best;
}

float verticalRedSliverMask(vec2 uv, vec2 texel, float targetLines) {
    float n = max(targetLines, 1.0);
    float segmentHeight = clamp(global.V7_VRED_SEGMENT_X / n, texel.y, 0.25);
    float boundaryY = floor(uv.y / segmentHeight) * segmentHeight;

    float yTop = clamp(boundaryY - 0.5 * segmentHeight, 0.0, 1.0);
    float yBottom = clamp(boundaryY + 0.5 * segmentHeight, 0.0, 1.0);
    vec3 cTop = texture(Source, vec2(uv.x, yTop)).rgb;
    vec3 cBottom = texture(Source, vec2(uv.x, yBottom)).rgb;

    float rTop = rednessRatio(cTop);
    float rBottom = rednessRatio(cBottom);
    float redDelta = abs(rBottom - rTop);
    float threshold = max(global.V7_VRED_DELTA, 0.0001);
    float edge = smoothstep(threshold, threshold * 2.0, redDelta);

    float satLow = min(global.V7_MASK_SAT_LOW, global.V7_MASK_SAT_HIGH);
    float satHigh = max(global.V7_MASK_SAT_LOW, global.V7_MASK_SAT_HIGH);
    float satGate = smoothstep(satLow, satHigh, min(satAmount(cTop), satAmount(cBottom)));

    const vec3 LUMA = vec3(0.299, 0.587, 0.114);
    vec3 cCenter = texture(Source, clamp(uv, vec2(0.0), vec2(1.0))).rgb;
    float centerSat = satAmount(cCenter);
    float centerGate = smoothstep(global.V7_MASK_CENTER_SAT, global.V7_MASK_CENTER_SAT + 0.08, centerSat);
    float centerChroma = length(cCenter - vec3(dot(cCenter, LUMA)));
    float neutralGate = smoothstep(global.V7_MASK_NEUTRAL_REJECT, global.V7_MASK_NEUTRAL_REJECT + 0.10, centerChroma);

    float sliverBase = 1.0 / n;
    float sliverHeight = sliverBase + sliverBase * max(global.V7_VRED_SLIVER_EXTRA, 0.0);
    float localY = uv.y - boundaryY;
    float below = step(0.0, localY) * step(localY, sliverHeight);

    return edge * satGate * centerGate * neutralGate * below;
}

vec3 applyDotCrawlV7(vec3 linearColor, vec2 uv) {
    vec2 srcTexel = 1.0 / global.SourceSize.xy;
    float targetLines = getTargetLines();
    float scanlinePx = max(1.0, global.OutputSize.y / targetLines);
    float lineThicknessPx = max(0.5, global.V7_LINE_THICKNESS * scanlinePx);
    float coarseSpanPx = scanlinePx * max(global.V7_MASK_SAMPLE_SPAN, 0.5);
    float coarseStepX = clamp(coarseSpanPx / global.OutputSize.x, srcTexel.x, srcTexel.x * 16.0);

    int maskMode = clamp(int(floor(global.V7_MASK_MODE + 0.5)), 0, 1);
    float edgeMask = (maskMode == 0)
        ? transitionGrown(uv, srcTexel, coarseStepX)
        : verticalRedSliverMask(uv, srcTexel, targetLines);

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
