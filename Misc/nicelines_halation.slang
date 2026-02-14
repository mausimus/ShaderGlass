#version 450
// nicelines v9 (2026-02-14)

// === PASS 4: HALATION BLUR + FINAL OUTPUT ===
// Input A (Source): extracted highlights from pass 3 (linear)
// Input B (Mask):   base masked image from pass 2 (linear, via alias)
// Output: final image in sRGB

// === PARAMETERS ===
#pragma parameter HALATION_AMOUNT "HALATION Amount" 0.48 0.0 2.0 0.01
#pragma parameter HALATION_RADIUS "HALATION Radius" 12.0 0.0 12.0 0.1
#pragma parameter HALATION_BLUR_MODE "HALATION Blur Mode (Gaussian / Bokeh / DualKawase / GooeyDual)" 0.0 0.0 3.0 1.0
#pragma parameter DEBUG_SPLIT "DEBUG: Comparison Split (Off/Top=Input/Bot=Input)" 0.0 0.0 2.0 1.0

layout(std140, set = 0, binding = 0) uniform UBO {
    mat4 MVP;
    vec4 SourceSize;
    vec4 OriginalSize;
    vec4 OutputSize;
    uint FrameCount;
    float HALATION_AMOUNT;
    float HALATION_RADIUS;
    float HALATION_BLUR_MODE;
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
layout(set = 0, binding = 2) uniform sampler2D Source;  // Pass 3 output (extracted highlights)
layout(set = 0, binding = 3) uniform sampler2D Mask;    // Pass 2 output alias

float LinearToSrgb(float c) {
    return c <= 0.0031308 ? c * 12.92 : 1.055 * pow(c, 1.0 / 2.4) - 0.055;
}

vec3 LinearToSrgb3(vec3 c) {
    return vec3(LinearToSrgb(c.r), LinearToSrgb(c.g), LinearToSrgb(c.b));
}

vec3 sampleHighlightAt(vec2 uv) {
    return texture(Source, clamp(uv, vec2(0.0), vec2(1.0))).rgb;
}

// Heavier true convolution blur on a 5x5 kernel.
vec3 blurHighlightsGaussian(vec2 uv, float radiusPx, vec2 texel) {
    // Radius maps to sampling step; keeps response smooth as slider changes.
    float stepPx = max(0.35, radiusPx * 0.5);
    vec3 sum = vec3(0.0);
    float sumW = 0.0;

    for(int y = -2; y <= 2; y++) {
        for(int x = -2; x <= 2; x++) {
            vec2 offset = vec2(float(x), float(y));
            float d2 = dot(offset, offset);
            float w = exp(-d2 * 0.5);
            vec2 tapUV = uv + offset * stepPx * texel;
            sum += sampleHighlightAt(tapUV) * w;
            sumW += w;
        }
    }
    return sum / sumW;
}

// Wide circular bokeh-ish blur with many taps to reduce visible stamp artifacts.
vec3 blurHighlightsBokeh(vec2 uv, float radiusPx, vec2 texel) {
    const int TAP_COUNT = 24;
    const vec2 DISK[TAP_COUNT] = vec2[](
        vec2( 1.00,  0.00), vec2( 0.87,  0.50), vec2( 0.50,  0.87), vec2( 0.00,  1.00),
        vec2(-0.50,  0.87), vec2(-0.87,  0.50), vec2(-1.00,  0.00), vec2(-0.87, -0.50),
        vec2(-0.50, -0.87), vec2( 0.00, -1.00), vec2( 0.50, -0.87), vec2( 0.87, -0.50),
        vec2( 0.55,  0.00), vec2( 0.48,  0.28), vec2( 0.28,  0.48), vec2( 0.00,  0.55),
        vec2(-0.28,  0.48), vec2(-0.48,  0.28), vec2(-0.55,  0.00), vec2(-0.48, -0.28),
        vec2(-0.28, -0.48), vec2( 0.00, -0.55), vec2( 0.28, -0.48), vec2( 0.48, -0.28)
    );

    vec3 sum = sampleHighlightAt(uv);
    float sumW = 1.0;

    for(int i = 0; i < TAP_COUNT; i++) {
        float d = length(DISK[i]);
        float w = (d > 0.8) ? 0.65 : 1.0;
        vec2 tapUV = uv + DISK[i] * radiusPx * texel;
        sum += sampleHighlightAt(tapUV) * w;
        sumW += w;
    }
    return sum / sumW;
}

// Dual-Kawase-style approximation in one pass:
// combine a small "downsample-style" diagonal gather and a wider "upsample-style" gather.
vec3 blurHighlightsDualKawase(vec2 uv, float radiusPx, vec2 texel) {
    float rDown = max(0.6, radiusPx * 0.65);
    float rUp = max(0.9, radiusPx * 1.35);

    // Kawase-style diagonal gather (downsample-ish stage)
    vec2 dd = texel * rDown;
    vec3 down = vec3(0.0);
    down += sampleHighlightAt(uv + vec2( dd.x,  dd.y));
    down += sampleHighlightAt(uv + vec2(-dd.x,  dd.y));
    down += sampleHighlightAt(uv + vec2( dd.x, -dd.y));
    down += sampleHighlightAt(uv + vec2(-dd.x, -dd.y));
    down += sampleHighlightAt(uv) * 0.5;
    down *= 1.0 / 4.5;

    vec2 du = texel * rUp;
    vec3 up = vec3(0.0);
    up += sampleHighlightAt(uv + vec2( du.x, 0.0));
    up += sampleHighlightAt(uv + vec2(-du.x, 0.0));
    up += sampleHighlightAt(uv + vec2(0.0,  du.y));
    up += sampleHighlightAt(uv + vec2(0.0, -du.y));
    up += sampleHighlightAt(uv + vec2( du.x,  du.y));
    up += sampleHighlightAt(uv + vec2(-du.x,  du.y));
    up += sampleHighlightAt(uv + vec2( du.x, -du.y));
    up += sampleHighlightAt(uv + vec2(-du.x, -du.y));
    up *= 1.0 / 8.0;

    return down * 0.55 + up * 0.45;
}

// Gooey stacked Dual-Kawase-style blur: multiple strengths for natural roll-off.
vec3 blurHighlightsGooeyDual(vec2 uv, float radiusPx, vec2 texel) {
    vec3 s1 = blurHighlightsDualKawase(uv, radiusPx * 0.70, texel);
    vec3 s2 = blurHighlightsDualKawase(uv, radiusPx * 1.35, texel);
    vec3 s3 = blurHighlightsDualKawase(uv, radiusPx * 2.25, texel);
    return s1 * 0.50 + s2 * 0.33 + s3 * 0.17;
}

void main() {
    vec3 baseLinear = texture(Mask, vTexCoord).rgb;

    // === COMPARISON SPLIT: show un-halated input ===
    int splitMode = int(global.DEBUG_SPLIT + 0.5);
    bool showRawHere = false;
    if(splitMode == 1 && vTexCoord.y < 0.5) showRawHere = true;
    if(splitMode == 2 && vTexCoord.y >= 0.5) showRawHere = true;
    if(showRawHere) {
        FragColor = vec4(LinearToSrgb3(baseLinear), 1.0);
        return;
    }

    float amount = global.HALATION_AMOUNT;
    float radius = global.HALATION_RADIUS;
    int blurMode = int(global.HALATION_BLUR_MODE + 0.5);

    if(amount <= 0.0001 || radius <= 0.0001) {
        FragColor = vec4(LinearToSrgb3(baseLinear), 1.0);
        return;
    }

    vec2 texel = 1.0 / global.OutputSize.xy;

    vec3 blurTight;
    vec3 blurWide;
    if(blurMode == 3) {
        // Gooey stacked dual-kawase style: smoothest falloff, heaviest
        blurTight = blurHighlightsGooeyDual(vTexCoord, radius, texel);
        blurWide = blurHighlightsGooeyDual(vTexCoord, radius * 1.6, texel);
    } else if(blurMode == 2) {
        // Dual-Kawase-style approximation
        blurTight = blurHighlightsDualKawase(vTexCoord, radius, texel);
        blurWide = blurHighlightsDualKawase(vTexCoord, radius * 1.8, texel);
    } else if(blurMode == 1) {
        // Bokeh-ish: widest, soft circular halos
        blurTight = blurHighlightsBokeh(vTexCoord, radius, texel);
        blurWide = blurHighlightsBokeh(vTexCoord, radius * 1.9, texel);
    } else {
        // Gaussian mode (0) and default fallback
        blurTight = blurHighlightsGaussian(vTexCoord, radius, texel);
        blurWide = blurHighlightsGaussian(vTexCoord, radius * 1.8, texel);
    }
    vec3 halation = blurTight * 0.7 + blurWide * 0.3;

    vec3 outLinear = baseLinear + halation * amount;
    FragColor = vec4(LinearToSrgb3(outLinear), 1.0);
}
