#version 450
// nicelines v2 (2026-02-13)

// === PASS 2: SHADOW MASK ===
// This pass handles: pixel swap mode, triad phosphor mask
// Input: scanline image from pass 1 (linear space, float framebuffer)
// Output: final image converted to sRGB for display

// === SHADOW MASK PARAMETERS ===
#pragma parameter SUBPIXEL_NOISE "SHADOW MASK Off/Swap/Triad/Dev" 3.0 0.0 3.0 1.0
#pragma parameter SHADOW_STRENGTH "Shadow Mask STRENGTH (1-3)" 2.0 1.0 3.0 1.0
#pragma parameter SHIMMER_NOISE "Shimmering NOISE Effect (Requires Shadow Mask Effect ON)" 0.0 0.0 1.0 1.0
#pragma parameter DEBUG_STAGES "DEBUG: Show Processing Stages Side-by-Side" 1.0 0.0 1.0 1.0

// === TRIAD PARAMETERS ===
#pragma parameter PHOSPHOR_WIDTH "TRIAD Phosphor Width (px)" 1.0 1.0 20.0 1.0
#pragma parameter TRIAD_HEIGHT "TRIAD Height (px)" 3.0 1.0 20.0 1.0
#pragma parameter SPILLOVER_MODE "TRIAD Spillover (Proportional/Curved)" 0.0 0.0 1.0 1.0
#pragma parameter SPILLOVER_SHAPE "TRIAD Proportional Spillover Shape (0=concentrated, 1=spread)" 0.5 0.0 1.0 0.05
#pragma parameter CURVE_PRIMARY "TRIAD Curve PRIMARY (0=linear, -=concave, +=convex)" -0.3 -3.0 3.0 0.1
#pragma parameter CURVE_SECONDARY "TRIAD Curve SECONDARY (0=linear, -=concave, +=convex)" 0.0 -3.0 3.0 0.1
#pragma parameter CURVE_TERTIARY "TRIAD Curve TERTIARY (0=linear, -=concave, +=convex)" 0.3 -3.0 3.0 0.1
#pragma parameter PHOSPHOR_GRADATION "TRIAD Vertical Phosphor Gradation" 1.0 0.0 1.0 1.0

// === DEV MODE PARAMETERS ===
#pragma parameter DEV_STAGE "DEV Stage (0=flat 1=distrib 2=boost 3=gradation)" 0.0 0.0 3.0 1.0

layout(std140, set = 0, binding = 0) uniform UBO {
    mat4 MVP;
    vec4 SourceSize;
    vec4 OriginalSize;
    vec4 OutputSize;
    uint FrameCount;
    float SUBPIXEL_NOISE;
    float SHADOW_STRENGTH;
    float SHIMMER_NOISE;
    float DEBUG_STAGES;
    float PHOSPHOR_WIDTH;
    float TRIAD_HEIGHT;
    float SPILLOVER_MODE;
    float SPILLOVER_SHAPE;
    float CURVE_PRIMARY;
    float CURVE_SECONDARY;
    float CURVE_TERTIARY;
    float PHOSPHOR_GRADATION;
    float DEV_STAGE;
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
layout(set = 0, binding = 2) uniform sampler2D Source;  // Pass 1 output (scanlines, linear space)

// Linear to sRGB conversion (only needed at final output)
float LinearToSrgb(float c) {
    return c <= 0.0031308 ? c * 12.92 : 1.055 * pow(c, 1.0/2.4) - 0.055;
}

vec3 LinearToSrgb3(vec3 c) {
    return vec3(LinearToSrgb(c.r), LinearToSrgb(c.g), LinearToSrgb(c.b));
}

void main() {
    vec2 px = vTexCoord * global.OutputSize.xy;

    // Debug mode: left 2/3 shows previous passes without mask
    if(global.DEBUG_STAGES > 0.5 && vTexCoord.x < (2.0 / 3.0)) {
        vec3 passthrough = texture(Source, vTexCoord).rgb;
        FragColor = vec4(LinearToSrgb3(passthrough), 1.0);
        return;
    }

    // Mode selection: 0=off, 1=swap, 2=triad, 3=dev
    int shadowMode = int(global.SUBPIXEL_NOISE + 0.5);

    // === MODE 0: No shadow mask - passthrough ===
    if(shadowMode == 0) {
        vec3 color = texture(Source, vTexCoord).rgb;
        FragColor = vec4(LinearToSrgb3(color), 1.0);
        return;
    }

    // === MODE 1: Pixel swap shadow mask ===
    if(shadowMode == 1) {
        // Seed: random each frame if shimmer enabled, otherwise fixed
        float seed = (global.SHIMMER_NOISE > 0.5) ? float(global.FrameCount) : 0.0;

        // Swap range directly from strength setting (1, 2, or 3)
        float swapRange = global.SHADOW_STRENGTH;

        // Calculate pixels per scanline (approximate from source/output ratio)
        float pixelsPerScanline = global.OutputSize.y / global.SourceSize.y;

        vec2 srcPx = px;

        if(swapRange > 0.5) {
            // Phase height is a factor of scanline pitch, rounded to integer fat-pixels (locked at 0.8)
            float phaseH = max(1.0, floor(pixelsPerScanline * 0.8 + 0.5));

            float groupWidth = swapRange * 2.0;
            float groupIdx = floor(px.x / groupWidth);
            float withinGroup = mod(px.x, groupWidth);

            float rowPhase = floor(px.y / phaseH);

            float swapOffset = 0.0;
            if(mod(rowPhase, 2.0) < 1.0) {
                swapOffset = groupWidth - 1.0 - 2.0 * withinGroup;
            } else {
                swapOffset = mod(withinGroup + swapRange, groupWidth) - withinGroup;
            }

            float chaos = sin((groupIdx + rowPhase) * 2.718 + seed) * swapRange * 0.5;
            swapOffset += chaos;

            srcPx.x += swapOffset;
        }

        // Convert displaced pixel position to UV and sample
        vec2 displacedUV = srcPx / global.OutputSize.xy;
        displacedUV = clamp(displacedUV, vec2(0.0), vec2(1.0));

        vec3 color = texture(Source, displacedUV).rgb;
        FragColor = vec4(LinearToSrgb3(color), 1.0);
        return;
    }

    // === MODE 3: Dev mode — incremental triad build-up ===
    // Shares triad geometry and area sampling with mode 2,
    // but applies features one at a time controlled by DEV_STAGE:
    //   0 = flat avgColor (baseline — should match input brightness/color exactly)
    //   1 = proportional distribution only (no boost, no gradation)
    //   2 = distribution + Lottes brightness boost
    //   3 = distribution + boost + vertical gradation
    bool devMode = (shadowMode == 3);
    int devStage = devMode ? int(global.DEV_STAGE + 0.5) : 99;

    // === MODE 2 & 3: Triad-based shadow mask ===
    float phosphorW = global.PHOSPHOR_WIDTH;
    float triadW = phosphorW * 3.0;  // Total triad width = 3 phosphors
    float triadH = global.TRIAD_HEIGHT;

    // Which triad column are we in?
    float triadIdxX = floor(px.x / triadW);
    float withinTriadX = mod(px.x, triadW);
    int phosphorIdx = int(floor(withinTriadX / phosphorW));  // 0=left, 1=middle, 2=right
    phosphorIdx = min(phosphorIdx, 2);  // Clamp to valid range

    // Vertical zigzag offset for alternating columns (when triad height > 1)
    float zigzagOffset = 0.0;
    if(triadH > 1.0 && mod(triadIdxX, 2.0) >= 1.0) {
        zigzagOffset = ceil(triadH / 2.0);
    }

    // Apply zigzag to Y position before calculating triad row
    float adjustedPxY = px.y - zigzagOffset;
    float triadIdxY = floor(adjustedPxY / triadH);

    // === AREA SAMPLING: Integrate the ENTIRE triad footprint ===
    // Triad bounds in output pixel space (with zigzag applied)
    float triadLeftPx = triadIdxX * triadW;
    float triadTopPx = triadIdxY * triadH + zigzagOffset;

    // Sample grid: one sample per output pixel in the triad
    int samplesX = max(int(triadW), 1);
    int samplesY = max(int(triadH), 1);

    vec3 totalEnergy = vec3(0.0);
    float sampleCount = 0.0;

    for(int sy = 0; sy < samplesY; sy++) {
        for(int sx = 0; sx < samplesX; sx++) {
            float samplePxX = triadLeftPx + float(sx) + 0.5;
            float samplePxY = triadTopPx + float(sy) + 0.5;

            vec2 sampleUV = vec2(samplePxX, samplePxY) / global.OutputSize.xy;
            sampleUV = clamp(sampleUV, vec2(0.0), vec2(1.0));

            // Input is already in linear space from pass 1
            vec3 sampleColor = texture(Source, sampleUV).rgb;

            totalEnergy += sampleColor;
            sampleCount += 1.0;
        }
    }

    // Average processed color in the triad area
    vec3 avgColor = totalEnergy / sampleCount;

    // === DEV STAGE 0: Flat passthrough (baseline brightness/color check) ===
    if(devMode && devStage == 0) {
        FragColor = vec4(LinearToSrgb3(avgColor), 1.0);
        return;
    }

    // Total energy to distribute across 3 phosphors (multiply by 3)
    float totalRed = avgColor.r * 3.0;
    float totalGreen = avgColor.g * 3.0;
    float totalBlue = avgColor.b * 3.0;

    // Spillover distribution: compute per-phosphor values for ALL 3 phosphors
    // Left phosphor (idx 0), Middle phosphor (idx 1), Right phosphor (idx 2)
    vec3 phosphorLeft, phosphorMiddle, phosphorRight;

    // Dev mode always uses proportional spillover for isolation
    if(devMode || global.SPILLOVER_MODE < 0.5) {
        // === PROPORTIONAL SPILLOVER ===
        // Energy-preserving weighted distribution controlled by shape slider
        // Shape 0 = concentrated (high exponent), Shape 1 = spread (low exponent)
        float exponent = mix(4.0, 0.5, global.SPILLOVER_SHAPE);

        // RED: primary=left, secondary=middle (adjacent), tertiary=right (far)
        float rNorm = clamp(totalRed / 3.0, 0.0, 1.0);
        float rWpri = 1.0;
        float rWsec = pow(rNorm, exponent);
        float rWter = pow(rNorm, exponent * 2.0);
        float rSum = rWpri + rWsec + rWter;
        float redLeft   = totalRed * rWpri / rSum;
        float redMiddle = totalRed * rWsec / rSum;
        float redRight  = totalRed * rWter / rSum;

        // GREEN: primary=middle, secondary=left (adjacent), tertiary=right (far)
        // Uses same pri/sec/ter chain as R and B for consistent white balance
        float gNorm = clamp(totalGreen / 3.0, 0.0, 1.0);
        float gWpri = 1.0;
        float gWsec = pow(gNorm, exponent);
        float gWter = pow(gNorm, exponent * 2.0);
        float gSum = gWpri + gWsec + gWter;
        float greenMiddle = totalGreen * gWpri / gSum;
        float greenLeft   = totalGreen * gWsec / gSum;
        float greenRight  = totalGreen * gWter / gSum;

        // BLUE: primary=right, secondary=middle (adjacent), tertiary=left (far)
        float bNorm = clamp(totalBlue / 3.0, 0.0, 1.0);
        float bWpri = 1.0;
        float bWsec = pow(bNorm, exponent);
        float bWter = pow(bNorm, exponent * 2.0);
        float bSum = bWpri + bWsec + bWter;
        float blueRight  = totalBlue * bWpri / bSum;
        float blueMiddle = totalBlue * bWsec / bSum;
        float blueLeft   = totalBlue * bWter / bSum;

        // Assemble per-phosphor vectors
        phosphorLeft   = vec3(redLeft,   greenLeft,   blueLeft);
        phosphorMiddle = vec3(redMiddle, greenMiddle, blueMiddle);
        phosphorRight  = vec3(redRight,  greenRight,  blueRight);
    } else {
        // === CURVED SPILLOVER ===
        float expPrimary = pow(2.0, global.CURVE_PRIMARY);
        float expSecondary = pow(2.0, global.CURVE_SECONDARY);
        float expTertiary = pow(2.0, global.CURVE_TERTIARY);

        // RED: primary=left, secondary=middle (adjacent), tertiary=right (far)
        float rNorm = totalRed / 3.0;
        float redLeft = pow(clamp(rNorm, 0.0, 1.0), expPrimary);
        float redMiddle = pow(clamp(rNorm, 0.0, 1.0), expSecondary);
        float redRight = pow(clamp(rNorm, 0.0, 1.0), expTertiary);

        // GREEN: primary=middle, secondary=left (adjacent), tertiary=right (far)
        float gNorm = totalGreen / 3.0;
        float greenMiddle = pow(clamp(gNorm, 0.0, 1.0), expPrimary);
        float greenLeft = pow(clamp(gNorm, 0.0, 1.0), expSecondary);
        float greenRight = pow(clamp(gNorm, 0.0, 1.0), expTertiary);

        // BLUE: primary=right, secondary=middle (adjacent), tertiary=left (far)
        float bNorm = totalBlue / 3.0;
        float blueRight = pow(clamp(bNorm, 0.0, 1.0), expPrimary);
        float blueMiddle = pow(clamp(bNorm, 0.0, 1.0), expSecondary);
        float blueLeft = pow(clamp(bNorm, 0.0, 1.0), expTertiary);

        // Assemble per-phosphor vectors
        phosphorLeft   = vec3(redLeft,   greenLeft,   blueLeft);
        phosphorMiddle = vec3(redMiddle, greenMiddle, blueMiddle);
        phosphorRight  = vec3(redRight,  greenRight,  blueRight);
    }

    // === DEV STAGE 1: Distribution only, no boost, no gradation ===
    if(devMode && devStage == 1) {
        vec3 phosphorEnergy;
        if(phosphorIdx == 0) {
            phosphorEnergy = phosphorLeft;
        } else if(phosphorIdx == 1) {
            phosphorEnergy = phosphorMiddle;
        } else {
            phosphorEnergy = phosphorRight;
        }
        FragColor = vec4(LinearToSrgb3(phosphorEnergy), 1.0);
        return;
    }

    // === LOTTES-STYLE NORMALIZATION BOOST ===
    // NOTE: For proportional mode, this boost is mathematically 1.0 (a no-op)
    // because the weighted division already conserves total energy exactly.
    // For curved mode, it corrects the non-conserving pow() outputs.
    // Proportional dimness is a perceptual issue (visible triad structure),
    // not a mathematical one — needs a different fix (TBD).
    vec3 spatialAvg = (phosphorLeft + phosphorMiddle + phosphorRight) / 3.0;
    vec3 boost = vec3(1.0);
    boost.r = (spatialAvg.r > 1e-6) ? (avgColor.r / spatialAvg.r) : 1.0;
    boost.g = (spatialAvg.g > 1e-6) ? (avgColor.g / spatialAvg.g) : 1.0;
    boost.b = (spatialAvg.b > 1e-6) ? (avgColor.b / spatialAvg.b) : 1.0;

    // Apply boost to all phosphors
    phosphorLeft   *= boost;
    phosphorMiddle *= boost;
    phosphorRight  *= boost;

    // === DEV STAGE 2: Distribution + boost, no gradation ===
    if(devMode && devStage == 2) {
        vec3 phosphorEnergy;
        if(phosphorIdx == 0) {
            phosphorEnergy = phosphorLeft;
        } else if(phosphorIdx == 1) {
            phosphorEnergy = phosphorMiddle;
        } else {
            phosphorEnergy = phosphorRight;
        }
        FragColor = vec4(LinearToSrgb3(phosphorEnergy), 1.0);
        return;
    }

    // Select the energy budget for this phosphor (stage 3 / normal mode 2)
    vec3 phosphorEnergy;
    if(phosphorIdx == 0) {
        phosphorEnergy = phosphorLeft;
    } else if(phosphorIdx == 1) {
        phosphorEnergy = phosphorMiddle;
    } else {
        phosphorEnergy = phosphorRight;
    }

    // === VERTICAL PHOSPHOR GRADATION ===
    // Redistribute partial phosphor energy vertically based on scanline brightness rank
    vec3 finalColor;
    int numRows = max(int(triadH), 1);

    if(global.PHOSPHOR_GRADATION > 0.5 && numRows > 1) {
        // Current row within the triad
        float rowInTriad = floor(px.y - triadTopPx);
        rowInTriad = clamp(rowInTriad, 0.0, float(numRows - 1));
        int myRowIdx = int(rowInTriad);

        // Sample X position: center of this phosphor column
        float sampleX = triadLeftPx + float(phosphorIdx) * phosphorW + phosphorW * 0.5;

        // Get brightness at each row in this phosphor column
        // and compute rank of current row (0 = brightest)
        float myPxY = triadTopPx + rowInTriad + 0.5;
        vec2 myUV = vec2(sampleX, myPxY) / global.OutputSize.xy;
        myUV = clamp(myUV, vec2(0.0), vec2(1.0));
        float myLuma = dot(texture(Source, myUV).rgb, vec3(0.2126, 0.7152, 0.0722));

        int myRank = 0;
        for(int r = 0; r < numRows; r++) {
            if(r == myRowIdx) continue;
            float rowPxY = triadTopPx + float(r) + 0.5;
            vec2 rowUV = vec2(sampleX, rowPxY) / global.OutputSize.xy;
            rowUV = clamp(rowUV, vec2(0.0), vec2(1.0));
            float rowLuma = dot(texture(Source, rowUV).rgb, vec3(0.2126, 0.7152, 0.0722));
            // Higher luma = better rank (lower number); tiebreak by row index
            if(rowLuma > myLuma || (rowLuma == myLuma && r < myRowIdx)) {
                myRank++;
            }
        }

        // Linear energy distribution by rank:
        // Weight for rank k = (N - k), sum of weights = N*(N+1)/2
        // Scale factor redistributes so total across all rows = phosphorEnergy * N
        float weight = float(numRows - myRank);
        float sumWeights = float(numRows * (numRows + 1)) / 2.0;
        float scaleFactor = weight * float(numRows) / sumWeights;

        // Apply gradation only to partial phosphors (energy < 1.0 for that channel)
        // Fully saturated channels stay flat
        finalColor.r = (phosphorEnergy.r >= 1.0) ? phosphorEnergy.r : phosphorEnergy.r * scaleFactor;
        finalColor.g = (phosphorEnergy.g >= 1.0) ? phosphorEnergy.g : phosphorEnergy.g * scaleFactor;
        finalColor.b = (phosphorEnergy.b >= 1.0) ? phosphorEnergy.b : phosphorEnergy.b * scaleFactor;
    } else {
        // No gradation — flat phosphor
        finalColor = phosphorEnergy;
    }

    // Convert from linear to sRGB for final display output
    FragColor = vec4(LinearToSrgb3(finalColor), 1.0);
}
