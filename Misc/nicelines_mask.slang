#version 450
// nicelines v7 (2026-02-13)

// === PASS 2: SHADOW MASK ===
// Aperture-grille-style phosphor triad mask with clip-safe energy distribution.
// Input: scanline image from pass 1 (linear space, float framebuffer)
// Output: final image converted to sRGB for display
//
// Algorithm: Hard-primary distribution with global clip-avoidance blend.
// Each phosphor starts as pure R, G, or B (×3 energy). When the brightest
// channel would exceed 1.0, ALL phosphors blend toward avgColor by the
// same factor, preserving both energy conservation and color balance.
// Result: strong R/G/B separation in darks, graceful desaturation in brights.
//
// TODO: investigate shimmering/noise effect for shadow mask
// TODO: investigate different phosphor patterns (dot trios, slot mask) vs current aperture grille
// TODO: investigate variable-width phosphors — each phosphor's width proportional to its
//       channel energy in the triad area. Per-channel independent grids reading from shared
//       triad input areas. Would eliminate clipping naturally (width encodes energy, not brightness).

// === PARAMETERS ===
#pragma parameter SHADOW_MASK "Shadow Mask (Off / Standard / Gutter)" 1.0 0.0 2.0 1.0
#pragma parameter PHOSPHOR_WIDTH "Phosphor Width (px)" 1.0 1.0 2.0 1.0
#pragma parameter PHOSPHOR_HEIGHT "Phosphor Height (px)" 3.0 2.0 4.0 1.0
#pragma parameter PHOSPHOR_GRADATION "Vertical Phosphor Gradation" 0.0 0.0 1.0 1.0
#pragma parameter DEBUG_STAGES "DEBUG: Show Processing Stages Side-by-Side" 1.0 0.0 1.0 1.0
#pragma parameter DEBUG_SPLIT "DEBUG: Comparison Split (Off/Top=Input/Bot=Input)" 0.0 0.0 2.0 1.0

layout(std140, set = 0, binding = 0) uniform UBO {
    mat4 MVP;
    vec4 SourceSize;
    vec4 OriginalSize;
    vec4 OutputSize;
    uint FrameCount;
    float SHADOW_MASK;
    float PHOSPHOR_WIDTH;
    float PHOSPHOR_HEIGHT;
    float PHOSPHOR_GRADATION;
    float DEBUG_STAGES;
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

    // === COMPARISON SPLIT ===
    // Mode 1: top half = raw input (no mask), bottom half = processed
    // Mode 2: bottom half = raw input, top half = processed
    int splitMode = int(global.DEBUG_SPLIT + 0.5);
    bool showRawHere = false;
    if(splitMode == 1 && vTexCoord.y < 0.5) showRawHere = true;
    if(splitMode == 2 && vTexCoord.y >= 0.5) showRawHere = true;

    if(showRawHere) {
        vec3 raw = texture(Source, vTexCoord).rgb;
        FragColor = vec4(LinearToSrgb3(raw), 1.0);
        return;
    }

    // === DEBUG SIDE-BY-SIDE ===
    // Left 2/3 shows previous passes without mask
    if(global.DEBUG_STAGES > 0.5 && vTexCoord.x < (2.0 / 3.0)) {
        vec3 passthrough = texture(Source, vTexCoord).rgb;
        FragColor = vec4(LinearToSrgb3(passthrough), 1.0);
        return;
    }

    int shadowMaskMode = int(global.SHADOW_MASK + 0.5);

    // === MODE 0: No shadow mask — passthrough ===
    if(shadowMaskMode == 0) {
        vec3 color = texture(Source, vTexCoord).rgb;
        FragColor = vec4(LinearToSrgb3(color), 1.0);
        return;
    }

    // === MODE 2: GUTTER MASK (fixed 3x4) ===
    // Layout per triad:
    //   Rows 0-2: RGB columns (1x3 each), zigzag offset by +2 px every other column group
    //   Row 3:    K gutter (3x1) shared overflow row
    // Channel distribution per triad channel c:
    //   primary(c)   = min(1, 4c)
    //   secondary(c) = clamp((4c - 1)/2, 0, 1)
    //   gutter(c)    = clamp(4c - 3, 0, 1)
    // This keeps gutter black until c > 0.75.
    if(shadowMaskMode == 2) {
        float triadW = 3.0;
        float triadH = 4.0;

        float triadIdxX = floor(px.x / triadW);
        float withinTriadX = mod(px.x, triadW);
        int colIdx = min(int(floor(withinTriadX)), 2);  // 0=R col, 1=G col, 2=B col

        float zigzagOffset = 0.0;
        if(mod(triadIdxX, 2.0) >= 1.0) {
            zigzagOffset = 2.0;
        }

        float adjustedPxY = px.y - zigzagOffset;
        float triadIdxY = floor(adjustedPxY / triadH);
        float triadLeftPx = triadIdxX * triadW;
        float triadTopPx = triadIdxY * triadH + zigzagOffset;

        float withinTriadY = mod(adjustedPxY, triadH);
        int rowIdx = clamp(int(floor(withinTriadY)), 0, 3);

        // Sample/average the full 3x4 triad input area
        vec3 totalEnergy = vec3(0.0);
        float sampleCount = 0.0;
        for(int sy = 0; sy < 4; sy++) {
            for(int sx = 0; sx < 3; sx++) {
                float samplePxX = triadLeftPx + float(sx) + 0.5;
                float samplePxY = triadTopPx + float(sy) + 0.5;
                vec2 sampleUV = vec2(samplePxX, samplePxY) / global.OutputSize.xy;
                sampleUV = clamp(sampleUV, vec2(0.0), vec2(1.0));
                vec3 sampleColor = texture(Source, sampleUV).rgb;
                totalEnergy += sampleColor;
                sampleCount += 1.0;
            }
        }
        vec3 avgColor = totalEnergy / sampleCount;

        // Per-channel piecewise distribution
        vec3 primaryVal   = min(avgColor * 4.0, vec3(1.0));
        vec3 secondaryVal = clamp((avgColor * 4.0 - vec3(1.0)) * 0.5, vec3(0.0), vec3(1.0));
        vec3 gutterVal    = clamp(avgColor * 4.0 - vec3(3.0), vec3(0.0), vec3(1.0));

        vec3 finalColor;
        if(rowIdx == 3) {
            // Bottom gutter row (K): only receives overflow beyond 0.75 per channel
            finalColor = gutterVal;
        } else {
            // RGB rows: primary column gets primaryVal, other columns get secondary spill
            finalColor.r = (colIdx == 0) ? primaryVal.r : secondaryVal.r;
            finalColor.g = (colIdx == 1) ? primaryVal.g : secondaryVal.g;
            finalColor.b = (colIdx == 2) ? primaryVal.b : secondaryVal.b;
        }

        FragColor = vec4(LinearToSrgb3(finalColor), 1.0);
        return;
    }

    // === MODE 1: STANDARD SHADOW MASK ===
    float phosphorW = global.PHOSPHOR_WIDTH;
    float triadW = phosphorW * 3.0;
    float triadH = global.PHOSPHOR_HEIGHT;

    // Which triad column are we in?
    float triadIdxX = floor(px.x / triadW);
    float withinTriadX = mod(px.x, triadW);
    int phosphorIdx = int(floor(withinTriadX / phosphorW));  // 0=R, 1=G, 2=B
    phosphorIdx = min(phosphorIdx, 2);

    // Vertical zigzag offset for alternating triad columns
    float zigzagOffset = 0.0;
    if(mod(triadIdxX, 2.0) >= 1.0) {
        zigzagOffset = ceil(triadH / 2.0);
    }

    // Apply zigzag to Y position before calculating triad row
    float adjustedPxY = px.y - zigzagOffset;
    float triadIdxY = floor(adjustedPxY / triadH);

    // === AREA SAMPLING: Average the entire triad footprint ===
    float triadLeftPx = triadIdxX * triadW;
    float triadTopPx = triadIdxY * triadH + zigzagOffset;

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

            vec3 sampleColor = texture(Source, sampleUV).rgb;
            totalEnergy += sampleColor;
            sampleCount += 1.0;
        }
    }

    vec3 avgColor = totalEnergy / sampleCount;

    // === HARD PRIMARY DISTRIBUTION WITH CLIP-SAFE BLEND ===
    //
    // Hard primaries: R phosphor = (R×3, 0, 0), G = (0, G×3, 0), B = (0, 0, B×3)
    //
    // When max(R,G,B)×3 > 1.0, framebuffer clips and energy is lost.
    // Fix: blend all 3 phosphors toward avgColor by a global factor t,
    // computed so the hottest channel exactly reaches 1.0.
    //
    // Global factor preserves energy: avg(phosphors) = avgColor at any t.
    //
    // Math: for primary channel c, result = c×3×(1-t) + c×t = c×(3-2t)
    //   Need c×(3-2t) ≤ 1.0 → t ≥ (3c-1)/(2c)
    //   Use t = max(0, (3m-1)/(2m)) where m = max(avgColor.r, avgColor.g, avgColor.b)

    float maxChan = max(avgColor.r, max(avgColor.g, avgColor.b));
    float t = 0.0;
    if(maxChan * 3.0 > 1.0) {
        t = (3.0 * maxChan - 1.0) / (2.0 * maxChan);
    }

    // Hard-primary for this phosphor position
    vec3 hardPri;
    if(phosphorIdx == 0) {
        hardPri = vec3(avgColor.r * 3.0, 0.0, 0.0);
    } else if(phosphorIdx == 1) {
        hardPri = vec3(0.0, avgColor.g * 3.0, 0.0);
    } else {
        hardPri = vec3(0.0, 0.0, avgColor.b * 3.0);
    }

    // Blend toward avgColor to prevent clipping
    vec3 phosphorEnergy = mix(hardPri, avgColor, t);

    // === VERTICAL PHOSPHOR GRADATION ===
    // Redistribute phosphor energy vertically by scanline brightness rank.
    vec3 finalColor;
    int numRows = max(int(triadH), 1);

    if(global.PHOSPHOR_GRADATION > 0.5 && numRows > 1) {
        float rowInTriad = floor(px.y - triadTopPx);
        rowInTriad = clamp(rowInTriad, 0.0, float(numRows - 1));
        int myRowIdx = int(rowInTriad);

        float sampleX = triadLeftPx + float(phosphorIdx) * phosphorW + phosphorW * 0.5;

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
            if(rowLuma > myLuma || (rowLuma == myLuma && r < myRowIdx)) {
                myRank++;
            }
        }

        float weight = float(numRows - myRank);
        float sumWeights = float(numRows * (numRows + 1)) / 2.0;
        float scaleFactor = weight * float(numRows) / sumWeights;

        finalColor.r = (phosphorEnergy.r >= 1.0) ? phosphorEnergy.r : phosphorEnergy.r * scaleFactor;
        finalColor.g = (phosphorEnergy.g >= 1.0) ? phosphorEnergy.g : phosphorEnergy.g * scaleFactor;
        finalColor.b = (phosphorEnergy.b >= 1.0) ? phosphorEnergy.b : phosphorEnergy.b * scaleFactor;
    } else {
        finalColor = phosphorEnergy;
    }

    // Convert from linear to sRGB for final display output
    FragColor = vec4(LinearToSrgb3(finalColor), 1.0);
}
