#version 450
// nicelines v10 (2026-02-14)

// === PASS 2: SHADOW MASK (GUTTER MODE) ===
// Gutter phosphor mask with channel spill behavior.
// Input: scanline image from pass 1 (linear space, float framebuffer)
// Output: masked image in linear space (for final post-processing pass)
//
// TODO: investigate shimmering/noise effect for shadow mask
// TODO: investigate different phosphor patterns (dot trios, slot mask) vs current aperture grille
// TODO: investigate variable-width phosphors — each phosphor's width proportional to its
//       channel energy in the triad area. Per-channel independent grids reading from shared
//       triad input areas. Would eliminate clipping naturally (width encodes energy, not brightness).

// === PARAMETERS ===
#pragma parameter SHADOW_MASK_MODE "Gutter Shadow Mask (Off / Single Width / Double Width)" 1.0 0.0 2.0 1.0
#pragma parameter GUTTER_CLIP_SAFE "Gutter Anti-Clip Blend" 0.35 0.0 1.0 0.01
#pragma parameter GUTTER_BRIGHTNESS "Gutter Brightness" 1.18 0.8 1.8 0.01
#pragma parameter DEBUG_STAGES "DEBUG: Show Processing Stages Side-by-Side" 0.0 0.0 1.0 1.0

layout(std140, set = 0, binding = 0) uniform UBO {
    mat4 MVP;
    vec4 SourceSize;
    vec4 OriginalSize;
    vec4 OutputSize;
    uint FrameCount;
    float SHADOW_MASK_MODE;
    float GUTTER_CLIP_SAFE;
    float GUTTER_BRIGHTNESS;
    float DEBUG_STAGES;
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

void main() {
    vec2 px = vTexCoord * global.OutputSize.xy;

    // === DEBUG SIDE-BY-SIDE ===
    // Left 2/3 shows previous passes without mask
    if(global.DEBUG_STAGES > 0.5 && vTexCoord.x < (2.0 / 3.0)) {
        vec3 passthrough = texture(Source, vTexCoord).rgb;
        FragColor = vec4(passthrough, 1.0);
        return;
    }

    // === MODE 0: No shadow mask — passthrough ===
    int maskMode = int(global.SHADOW_MASK_MODE + 0.5);
    if(maskMode == 0) {
        vec3 color = texture(Source, vTexCoord).rgb;
        FragColor = vec4(color, 1.0);
        return;
    }

    // === GUTTER MASK (fixed 3x4 or 6x4 when double width is on) ===
    // Layout per triad:
    //   Rows 0-2: RGB columns (1x3 each) or (2x3 each in double-width mode)
    //   Row 3:    K gutter (3x1) or (6x1 in double-width mode)
    //   Zigzag: every other triad column group offset by +2 px
    // Channel distribution per triad channel c:
    //   primary(c)   = min(1, 4c)
    //   secondary(c) = clamp((4c - 1)/2, 0, 1)
    //   gutter(c)    = clamp(4c - 3, 0, 1)
    // This keeps gutter black until c > 0.75.
    float phosphorW = (maskMode >= 2) ? 2.0 : 1.0;
    float triadW = phosphorW * 3.0;
    float triadH = 4.0;

    float triadIdxX = floor(px.x / triadW);
    float withinTriadX = mod(px.x, triadW);
    int colIdx = min(int(floor(withinTriadX / phosphorW)), 2);  // 0=R col, 1=G col, 2=B col

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

    // Sample/average the full triad input area
    int samplesX = max(int(triadW), 1);
    vec3 totalEnergy = vec3(0.0);
    float sampleCount = 0.0;
    for(int sy = 0; sy < 4; sy++) {
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

    // Per-channel piecewise distribution.
    // Factor 4 stays valid in both widths because each channel bucket area remains 1/4 of triad area.
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

    // Clip-safe blend (adapted from prior anti-clipping fix):
    // Blend masked output toward triad average by user amount.
    // This keeps triad-average energy stable while reducing dim/clippy appearance.
    float clipSafe = clamp(global.GUTTER_CLIP_SAFE, 0.0, 1.0);
    finalColor = mix(finalColor, avgColor, clipSafe);

    // Explicit post-mask brightness compensation for displays where
    // intra-triad redistribution is perceptually subtle.
    finalColor *= global.GUTTER_BRIGHTNESS;

    FragColor = vec4(finalColor, 1.0);
}
