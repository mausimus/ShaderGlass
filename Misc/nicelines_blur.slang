#version 450
// nicelines v16 (2026-02-14)

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
#pragma parameter SCANLINE_MODE "SCANLINES MODE (224p / 240p / Manual)" 1.0 0.0 2.0 1.0
#pragma parameter MANUAL_LINES "MANUAL Mode: Number of Scanlines" 360.0 120.0 480.0 5.0
#pragma parameter FINETUNE_LINES "FINETUNE Number of Scanlines" 0.0 -5.0 5.0 1.0
#pragma parameter AUTO_LINES "ROUND Number of Scanlines to Nearest Multiple of Resolution" 0.0 0.0 1.0 1.0
#pragma parameter TEMPORAL_LINES "AUTO 220-260 (Discrete, Off/On)" 1.0 0.0 1.0 1.0
#pragma parameter AUTO_LOCK_BIAS "AUTO Stability Bias (Higher = less switching)" 0.03 0.0 0.20 0.01
#pragma parameter AUTO_DEBUG_VIEW "AUTO DEBUG HUD (Off/Lines/Confidence)" 0.0 0.0 2.0 1.0
#pragma parameter DEBUG_SPLIT "DEBUG: Comparison Split (Off/Top=Input/Bot=Input)" 0.0 0.0 2.0 1.0

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
    float FINETUNE_LINES;
    float AUTO_LINES;
    float TEMPORAL_LINES;
    float AUTO_LOCK_BIAS;
    float AUTO_DEBUG_VIEW;
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
layout(set = 0, binding = 2) uniform sampler2D Source;
layout(set = 0, binding = 3) uniform sampler2D PassFeedback;

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

float lumaSrgb(vec3 c) {
    return dot(c, vec3(0.299, 0.587, 0.114));
}

float rowEdgeEnergy(float yNorm) {
    const int XS = 5;
    const float XPOS[XS] = float[](0.10, 0.28, 0.50, 0.72, 0.90);
    vec2 texel = 1.0 / global.SourceSize.xy;

    float edge = 0.0;
    for(int xi = 0; xi < XS; xi++) {
        vec2 uv = vec2(XPOS[xi], yNorm);
        float up = lumaSrgb(texture(Source, clamp(uv + vec2(0.0, texel.y), vec2(0.0), vec2(1.0))).rgb);
        float dn = lumaSrgb(texture(Source, clamp(uv - vec2(0.0, texel.y), vec2(0.0), vec2(1.0))).rgb);
        edge += abs(up - dn);
    }
    return edge / float(XS);
}

void detectActiveWindow(out float yMin, out float yMax) {
    const int YS = 56;
    float eMax = 0.0;

    for(int yi = 0; yi < YS; yi++) {
        float y = (float(yi) + 0.5) / float(YS);
        eMax = max(eMax, rowEdgeEnergy(y));
    }

    float threshold = max(0.006, eMax * 0.18);
    yMin = 0.0;
    yMax = 1.0;
    bool found = false;

    for(int yi = 0; yi < YS; yi++) {
        float y = (float(yi) + 0.5) / float(YS);
        float e = rowEdgeEnergy(y);
        if(e > threshold) {
            if(!found) {
                yMin = y;
                found = true;
            }
            yMax = y;
        }
    }

    if(!found || (yMax - yMin) < 0.08) {
        yMin = 0.0;
        yMax = 1.0;
    }
}

float signatureScore(float cand, float yMin, float yMax) {
    const float PI2 = 6.28318530718;
    const int YS = 52;

    float re1 = 0.0;
    float im1 = 0.0;
    float re2 = 0.0;
    float im2 = 0.0;
    float hit = 0.0;
    float off = 0.0;
    float totalEdge = 0.0;

    float srcH = global.SourceSize.y;
    float span = max(yMax - yMin, 1e-4);
    float freq = cand / srcH;

    for(int yi = 0; yi < YS; yi++) {
        float t = (float(yi) + 0.5) / float(YS);
        float y = mix(yMin, yMax, t);
        float rowEdge = rowEdgeEnergy(y);
        totalEdge += rowEdge;

        float yPix = y * srcH;
        float ph1 = PI2 * freq * yPix;
        float ph2 = PI2 * (2.0 * freq) * yPix;
        re1 += rowEdge * cos(ph1);
        im1 += rowEdge * sin(ph1);
        re2 += rowEdge * cos(ph2);
        im2 += rowEdge * sin(ph2);

        float cyc = fract((y - yMin) * cand / span);
        float dist = min(cyc, 1.0 - cyc);
        float wHit = clamp(1.0 - dist * 8.0, 0.0, 1.0);
        hit += rowEdge * wHit;
        off += rowEdge * (1.0 - wHit);
    }

    if(totalEdge <= 1e-5) {
        return 0.0;
    }

    float fundamental = length(vec2(re1, im1)) / totalEdge;
    float harmonic2 = length(vec2(re2, im2)) / totalEdge;
    float hitRatio = hit / max(hit + off, 1e-5);

    return fundamental * 0.70 + hitRatio * 0.45 - harmonic2 * 0.35;
}

void detectAutoStats(float srcH, out float bestLines, out float bestQ, out float secondQ) {
    bestLines = 240.0;
    bestQ = -1e9;
    secondQ = -1e9;

    float yMin;
    float yMax;
    detectActiveWindow(yMin, yMax);

    for(int cand = 220; cand <= 260; cand++) {
        float c = float(cand);
        float q = signatureScore(c, yMin, yMax);

        if(q > bestQ) {
            secondQ = bestQ;
            bestQ = q;
            bestLines = c;
        } else if(q > secondQ) {
            secondQ = q;
        }
    }
}

float getTemporalTargetLines(float manualTargetLines,
                             out float bestQ,
                             out float secondQ,
                             out float instantBestLines) {
    if(global.TEMPORAL_LINES <= 0.5) {
        bestQ = 0.0;
        secondQ = 0.0;
        instantBestLines = manualTargetLines;
        return manualTargetLines;
    }

    float srcH = global.SourceSize.y;
    float bestNow;
    detectAutoStats(srcH, bestNow, bestQ, secondQ);
    instantBestLines = bestNow;

    vec4 prevState = texture(PassFeedback, vec2(0.5));
    float prevLinesNorm = prevState.a;
    bool prevValid = prevLinesNorm > 0.001;
    if(!prevValid) {
        return bestNow;
    }

    float prevLines = clamp(round(prevLinesNorm * 1024.0), 220.0, 260.0);
    float yMin;
    float yMax;
    detectActiveWindow(yMin, yMax);
    float prevQ = signatureScore(prevLines, yMin, yMax);

    float bias = clamp(global.AUTO_LOCK_BIAS, 0.0, 0.20);
    if(prevQ + bias >= bestQ) {
        return prevLines;
    }
    return bestNow;
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
    // === COMPARISON SPLIT: pass through raw input (sRGB→linear) ===
    int splitMode = int(global.DEBUG_SPLIT + 0.5);
    bool showRawHere = false;
    if(splitMode == 1 && vTexCoord.y < 0.5) showRawHere = true;
    if(splitMode == 2 && vTexCoord.y >= 0.5) showRawHere = true;
    if(showRawHere) {
        // Pass through in linear space (downstream passes also skip)
        float linesNormRaw = clamp(getTargetLines() / 1024.0, 0.0, 1.0);
        FragColor = vec4(SrgbToLinear3(texture(Source, vTexCoord).rgb), linesNormRaw);
        return;
    }

    float targetLines;
    float bestQ = 0.0;
    float secondQ = 0.0;
    float instantBestLines = 0.0;
    if(global.TEMPORAL_LINES > 0.5) {
        vec4 prevState = texture(PassFeedback, vec2(0.5));
        float prevLinesNorm = prevState.a;
        float fallbackLines = (prevLinesNorm > 0.001) ? clamp(round(prevLinesNorm * 1024.0), 220.0, 260.0) : getTargetLines();

        vec2 probe = global.OutputSize.xy * 0.5;
        bool isProbePixel = abs(gl_FragCoord.x - probe.x) < 1.5 && abs(gl_FragCoord.y - probe.y) < 1.5;
        targetLines = isProbePixel ? getTemporalTargetLines(getTargetLines(), bestQ, secondQ, instantBestLines) : fallbackLines;
    } else {
        targetLines = getTargetLines();
    }
    float baselineScale = targetLines / global.SourceSize.y;
    float effectiveScale = clamp(baselineScale + global.BLUR_OFFSET, 0.02, 1.0);

    // Apply downscale + horizontal blur, output in linear space
    vec3 color = sampleWithHBlur(vTexCoord, effectiveScale, global.H_BLUR);

    int autoDbg = int(global.AUTO_DEBUG_VIEW + 0.5);
    if(autoDbg > 0) {
        float hudW = min(260.0, global.OutputSize.x * 0.35);
        float hudH = 26.0;
        bool inHud = (gl_FragCoord.x <= hudW && gl_FragCoord.y >= (global.OutputSize.y - hudH));

        if(inHud) {
            float hudLines = targetLines;
            float hudBestQ = bestQ;
            float hudSecondQ = secondQ;
            float hudInstantBest = instantBestLines;

            if(global.TEMPORAL_LINES > 0.5) {
                hudLines = getTemporalTargetLines(getTargetLines(), hudBestQ, hudSecondQ, hudInstantBest);
            }

            float x01 = clamp(gl_FragCoord.x / hudW, 0.0, 1.0);
            float linePos = clamp((hudLines - 220.0) / 40.0, 0.0, 1.0);
            float gap = max(0.0, hudBestQ - hudSecondQ);
            float gapVis = clamp(gap * 8.0, 0.0, 1.0);

            if(autoDbg == 1) {
                vec3 bg = vec3(0.08, 0.08, 0.10);
                vec3 fg = vec3(0.10, 0.75, 0.25);
                color = (x01 <= linePos) ? fg : bg;

                if(abs(x01 - linePos) < (1.5 / hudW)) {
                    color = vec3(1.0, 0.95, 0.2);
                }
            } else {
                vec3 bg = vec3(0.08, 0.08, 0.10);
                vec3 fg = vec3(0.20, 0.55, 0.95);
                color = (x01 <= gapVis) ? fg : bg;
            }
        }
    }

    float linesNorm = clamp(targetLines / 1024.0, 0.0, 1.0);

    FragColor = vec4(color, linesNorm);
}
