#version 450
// nicelines v7 dot crawl pass (2026-02-14)

// === PASS 1: DOT CRAWL MASK DEBUG (V-Red) ===
// Input: blurred linear-space image from pass 0
// Output: unchanged image or direct mask visualization for tuning

#pragma parameter V7_MASK_DEBUG "DOT CRAWL Mask View (Off / On)" 1.0 0.0 1.0 1.0
#pragma parameter V7_MASK_CENTER_SAT "DOT CRAWL Mask Center Sat Clamp" 0.10 0.0 0.6 0.01
#pragma parameter V7_MASK_SAT_LOW "DOT CRAWL Mask Sat Low" 0.08 0.0 0.6 0.01
#pragma parameter V7_MASK_SAT_HIGH "DOT CRAWL Mask Sat High" 0.32 0.0 0.8 0.01
#pragma parameter V7_MASK_NEUTRAL_REJECT "DOT CRAWL Mask Neutral Reject" 0.10 0.0 0.6 0.01
#pragma parameter V7_VRED_SEGMENT_X "DOT CRAWL V-Red Segment X/n" 1.0 0.25 4.0 0.05
#pragma parameter V7_VRED_DELTA "DOT CRAWL V-Red Delta Threshold" 0.18 0.01 1.0 0.01
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
    float V7_MASK_DEBUG;
    float V7_MASK_CENTER_SAT;
    float V7_MASK_SAT_LOW;
    float V7_MASK_SAT_HIGH;
    float V7_MASK_NEUTRAL_REJECT;
    float V7_VRED_SEGMENT_X;
    float V7_VRED_DELTA;
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

float satAmount(vec3 c) {
    return max(c.r, max(c.g, c.b)) - min(c.r, min(c.g, c.b));
}

float rednessRatio(vec3 c) {
    return c.r / max(c.g + c.b, 0.0001);
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

void main() {
    vec3 baseColor = texture(Source, vTexCoord).rgb;
    vec2 srcTexel = 1.0 / global.SourceSize.xy;
    float targetLines = getTargetLines();
    float mask = verticalRedSliverMask(vTexCoord, srcTexel, targetLines);

    if(global.V7_MASK_DEBUG > 0.5) {
        FragColor = vec4(vec3(clamp(mask, 0.0, 1.0)), 1.0);
    } else {
        FragColor = vec4(baseColor, 1.0);
    }
}
