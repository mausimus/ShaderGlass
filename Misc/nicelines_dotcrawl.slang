#version 450
// nicelines v7 dot crawl pass (2026-02-14)

// === PASS 1: DOT CRAWL MASK DEBUG (V-Red) ===
// Input: blurred linear-space image from pass 0
// Output: dot-crawl processed image or direct mask visualization for tuning

#pragma parameter DOT_CRAWL "DOT CRAWL v7 (Off / On)" 1.0 0.0 1.0 1.0
#pragma parameter DOT_CRAWL_INTENSITY "DOT CRAWL v7 Intensity" 6.0 0.0 6.0 0.01
#pragma parameter V7_LINE_THICKNESS "DOT CRAWL v7 Line Thickness" 0.4 0.1 1.0 0.05
#pragma parameter V7_SWAP_SIZE "DOT CRAWL v7 Swap Size" 1.0 0.0 8.0 0.1
#pragma parameter V7_SPEED "DOT CRAWL v7 Horizontal Speed" 1.1 0.0 3.0 0.05
#pragma parameter V7_MASK_DEBUG "DOT CRAWL Mask View (Off / On)" 0.0 0.0 1.0 1.0
#pragma parameter V7_VRED_SEGMENT_X "DOT CRAWL V-Red Segment X/n" 1.0 1.0 3.0 1.0
#pragma parameter V7_VRED_DELTA "DOT CRAWL V-Red Delta Threshold" 0.09 0.01 1.0 0.01
#pragma parameter V7_VRED_ONSET_BIAS "DOT CRAWL V-Red Black->Red Bias" 0.66 0.0 1.0 0.01
#pragma parameter V7_VRED_SLIVER_EXTRA "DOT CRAWL V-Red Sliver Extra" 0.0 0.0 3.0 0.05

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
    float V7_LINE_THICKNESS;
    float V7_SWAP_SIZE;
    float V7_SPEED;
    float V7_MASK_DEBUG;
    float V7_VRED_SEGMENT_X;
    float V7_VRED_DELTA;
    float V7_VRED_ONSET_BIAS;
    float V7_VRED_SLIVER_EXTRA;
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

float redSignal(vec3 c) {
    return max(c.r - 0.5 * (c.g + c.b), 0.0);
}

float redFraction(vec3 c) {
    return c.r / max(c.r + c.g + c.b, 0.0001);
}

vec3 sampleCellColor(float xCenter, float yCenter, float columnWidth, float segmentHeight) {
    float xOff = 0.25 * columnWidth;
    float yOff = 0.25 * segmentHeight;

    vec3 c0 = texture(Source, vec2(clamp(xCenter - xOff, 0.0, 1.0), clamp(yCenter - yOff, 0.0, 1.0))).rgb;
    vec3 c1 = texture(Source, vec2(clamp(xCenter,        0.0, 1.0), clamp(yCenter - yOff, 0.0, 1.0))).rgb;
    vec3 c2 = texture(Source, vec2(clamp(xCenter + xOff, 0.0, 1.0), clamp(yCenter - yOff, 0.0, 1.0))).rgb;

    vec3 c3 = texture(Source, vec2(clamp(xCenter - xOff, 0.0, 1.0), clamp(yCenter,        0.0, 1.0))).rgb;
    vec3 c4 = texture(Source, vec2(clamp(xCenter,        0.0, 1.0), clamp(yCenter,        0.0, 1.0))).rgb;
    vec3 c5 = texture(Source, vec2(clamp(xCenter + xOff, 0.0, 1.0), clamp(yCenter,        0.0, 1.0))).rgb;

    vec3 c6 = texture(Source, vec2(clamp(xCenter - xOff, 0.0, 1.0), clamp(yCenter + yOff, 0.0, 1.0))).rgb;
    vec3 c7 = texture(Source, vec2(clamp(xCenter,        0.0, 1.0), clamp(yCenter + yOff, 0.0, 1.0))).rgb;
    vec3 c8 = texture(Source, vec2(clamp(xCenter + xOff, 0.0, 1.0), clamp(yCenter + yOff, 0.0, 1.0))).rgb;

    return (c0 + c1 + c2 + c3 + c4 + c5 + c6 + c7 + c8) / 9.0;
}

float verticalRedEdgeAtBoundary(float boundaryY, float columnCenterX, float columnWidth, float segmentHeight) {
    float yTop = clamp(boundaryY - 0.5 * segmentHeight, 0.0, 1.0);
    float yBottom = clamp(boundaryY + 0.5 * segmentHeight, 0.0, 1.0);

    vec3 cTop = sampleCellColor(columnCenterX, yTop, columnWidth, segmentHeight);
    vec3 cBottom = sampleCellColor(columnCenterX, yBottom, columnWidth, segmentHeight);

    float fracTop = redFraction(cTop);
    float fracBottom = redFraction(cBottom);
    float fracDelta = abs(fracBottom - fracTop);

    float signalTop = redSignal(cTop);
    float signalBottom = redSignal(cBottom);
    float signalDelta = abs(signalBottom - signalTop);
    float maxSignal = max(signalTop, signalBottom);
    float onset = max(signalBottom - signalTop, 0.0);

    float threshold = max(global.V7_VRED_DELTA, 0.0001);
    float onsetBias = clamp(global.V7_VRED_ONSET_BIAS, 0.0, 1.0);
    float onsetTerm = onset * mix(1.0, 2.4, onsetBias);
    float textureTerm = signalDelta * mix(1.8, 0.7, onsetBias);
    float ratioTerm = fracDelta * mix(1.3, 0.8, onsetBias);
    float combinedDelta = max(onsetTerm, max(textureTerm, ratioTerm));
    float edge = smoothstep(threshold, threshold * 2.0, combinedDelta);
    float redPresence = smoothstep(0.05, 0.16, maxSignal);
    return edge * redPresence;
}

float verticalRedSliverMask(vec2 uv, vec2 texel, float targetLines) {
    float n = max(targetLines, 1.0);
    float segmentHeight = clamp(global.V7_VRED_SEGMENT_X / n, texel.y, 0.25);
    float columnWidth = clamp(1.0 / n, texel.x, 0.25);
    float colIndex = floor(uv.x / columnWidth);
    float columnCenterX = clamp((colIndex + 0.5) * columnWidth, 0.0, 1.0);

    float rowIndex = floor(uv.y / segmentHeight);
    float boundaryY = rowIndex * segmentHeight;

    float sliverRows = 1.0 + max(global.V7_VRED_SLIVER_EXTRA, 0.0);
    int maxRowsBack = clamp(int(ceil(sliverRows)) - 1, 0, 8);

    float edgeAndCoverage = 0.0;
    for(int k = 0; k <= 8; k++) {
        if(k <= maxRowsBack) {
            float candidateBoundaryY = boundaryY - float(k) * segmentHeight;
            float edge = verticalRedEdgeAtBoundary(candidateBoundaryY, columnCenterX, columnWidth, segmentHeight);

            float yTop = candidateBoundaryY;
            float yBottom = candidateBoundaryY + sliverRows * segmentHeight;
            float below = step(yTop, uv.y) * step(uv.y, yBottom);

            edgeAndCoverage = max(edgeAndCoverage, edge * below);
        }
    }

    return edgeAndCoverage;
}

vec3 applyDotCrawlV7(vec3 linearColor, vec2 uv, float mask, float targetLines) {
    vec2 srcTexel = 1.0 / global.SourceSize.xy;
    float scanlinePx = max(1.0, global.OutputSize.y / targetLines);
    float lineThicknessPx = max(0.5, global.V7_LINE_THICKNESS * scanlinePx);

    vec2 px = uv * global.OutputSize.xy;
    float travelPx = float(global.FrameCount) * global.V7_SPEED;

    float lineCoord = (px.x + travelPx + px.y) / lineThicknessPx;
    float lineId = floor(lineCoord);

    float pairId = floor(lineId / 2.0);
    float lineInPair = mod(lineId, 2.0);
    float pairSign = (mod(pairId, 2.0) < 1.0) ? -1.0 : 1.0;
    float intraSign = (lineInPair < 1.0) ? 1.0 : -1.0;
    float swapSign = pairSign * intraSign;

    vec2 swapDir = normalize(vec2(1.0, 1.0));
    float swapPx = global.V7_SWAP_SIZE * clamp(global.DOT_CRAWL_INTENSITY, 0.0, 6.0) * mask;
    vec2 swapUV = uv + swapDir * (swapSign * swapPx) * srcTexel;
    vec3 swapped = texture(Source, clamp(swapUV, vec2(0.0), vec2(1.0))).rgb;

    float y0 = dot(linearColor, vec3(0.299, 0.587, 0.114));
    float y1 = dot(swapped,     vec3(0.299, 0.587, 0.114));
    vec3 ch1 = swapped - vec3(y1);
    vec3 outColor = clamp(vec3(y0) + ch1, 0.0, 1.0);

    return mix(linearColor, outColor, clamp(mask, 0.0, 1.0));
}

void main() {
    vec3 baseColor = texture(Source, vTexCoord).rgb;
    vec2 srcTexel = 1.0 / global.SourceSize.xy;
    float targetLines = getTargetLines();
    float mask = verticalRedSliverMask(vTexCoord, srcTexel, targetLines);

    if(global.V7_MASK_DEBUG > 0.5) {
        FragColor = vec4(vec3(clamp(mask, 0.0, 1.0)), 1.0);
    } else {
        vec3 crawledColor = (global.DOT_CRAWL > 0.5)
            ? applyDotCrawlV7(baseColor, vTexCoord, mask, targetLines)
            : baseColor;
        FragColor = vec4(crawledColor, 1.0);
    }
}
