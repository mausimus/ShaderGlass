#version 450
// nicelines v8 (2026-02-14)

// === PASS 3: HALATION + FINAL OUTPUT ===
// Input: masked image from pass 2 (linear space)
// Output: final image in sRGB
//
// Halation model: extract near-clipped highlights per channel,
// blur them with two radii (tight + broad), and add back as a
// screen-side glass/film glow layer.

// === PARAMETERS ===
#pragma parameter HALATION_AMOUNT "HALATION Amount" 0.12 0.0 0.5 0.01
#pragma parameter HALATION_THRESHOLD "HALATION Threshold" 0.9 0.6 0.99 0.01
#pragma parameter HALATION_RADIUS "HALATION Radius" 2.0 0.0 12.0 0.1
#pragma parameter DEBUG_SPLIT "DEBUG: Comparison Split (Off/Top=Input/Bot=Input)" 0.0 0.0 2.0 1.0

layout(std140, set = 0, binding = 0) uniform UBO {
    mat4 MVP;
    vec4 SourceSize;
    vec4 OriginalSize;
    vec4 OutputSize;
    uint FrameCount;
    float HALATION_AMOUNT;
    float HALATION_THRESHOLD;
    float HALATION_RADIUS;
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
layout(set = 0, binding = 2) uniform sampler2D Source;  // Pass 2 output (linear)

float LinearToSrgb(float c) {
    return c <= 0.0031308 ? c * 12.92 : 1.055 * pow(c, 1.0 / 2.4) - 0.055;
}

vec3 LinearToSrgb3(vec3 c) {
    return vec3(LinearToSrgb(c.r), LinearToSrgb(c.g), LinearToSrgb(c.b));
}

// Near-clipped highlight extraction (per channel)
// Linear extraction gives consistent response across threshold values.
// At threshold=1.0, only values above 1.0 contribute.
vec3 extractHighlights(vec3 c, float threshold) {
    return max(c - vec3(threshold), vec3(0.0));
}

vec3 sampleHighlightAt(vec2 uv, float threshold) {
    vec3 src = texture(Source, clamp(uv, vec2(0.0), vec2(1.0))).rgb;
    return extractHighlights(src, threshold);
}

vec3 blurHighlights(vec2 uv, float threshold, float radiusPx, vec2 texel) {
    const int TAP_COUNT = 12;
    const vec2 OFFSETS[TAP_COUNT] = vec2[](
        vec2( 0.00, -1.00),
        vec2( 0.86, -0.50),
        vec2( 0.86,  0.50),
        vec2( 0.00,  1.00),
        vec2(-0.86,  0.50),
        vec2(-0.86, -0.50),
        vec2( 0.45, -0.20),
        vec2( 0.55,  0.30),
        vec2( 0.05,  0.60),
        vec2(-0.50,  0.25),
        vec2(-0.55, -0.25),
        vec2(-0.05, -0.60)
    );

    vec3 sum = sampleHighlightAt(uv, threshold);
    float sumW = 1.0;

    for(int i = 0; i < TAP_COUNT; i++) {
        float d = length(OFFSETS[i]);
        float w = exp(-d * d * 1.8);
        vec2 tapUV = uv + OFFSETS[i] * radiusPx * texel;
        sum += sampleHighlightAt(tapUV, threshold) * w;
        sumW += w;
    }
    return sum / sumW;
}

void main() {
    vec3 baseLinear = texture(Source, vTexCoord).rgb;

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
    float threshold = global.HALATION_THRESHOLD;
    float radius = global.HALATION_RADIUS;

    if(amount <= 0.0001 || radius <= 0.0001) {
        FragColor = vec4(LinearToSrgb3(baseLinear), 1.0);
        return;
    }

    vec2 texel = 1.0 / global.OutputSize.xy;

    // Two-scale smooth blur to avoid stamped/stepped ring artifacts
    vec3 blurTight = blurHighlights(vTexCoord, threshold, radius, texel);
    vec3 blurWide = blurHighlights(vTexCoord, threshold, radius * 2.2, texel);
    vec3 halation = blurTight * 0.7 + blurWide * 0.3;

    vec3 outLinear = baseLinear + halation * amount;
    FragColor = vec4(LinearToSrgb3(outLinear), 1.0);
}
