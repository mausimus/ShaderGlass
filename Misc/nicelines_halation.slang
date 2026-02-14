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
#pragma parameter HALATION_THRESHOLD "HALATION Threshold" 0.9 0.6 1.0 0.01
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
vec3 extractHighlights(vec3 c, float threshold) {
    vec3 t = smoothstep(vec3(threshold), vec3(1.0), c);
    return t * c;
}

vec3 sampleHighlightAt(vec2 uv, float threshold) {
    vec3 src = texture(Source, clamp(uv, vec2(0.0), vec2(1.0))).rgb;
    return extractHighlights(src, threshold);
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

    // Two-lobe isotropic-ish kernel via cardinal + diagonal taps
    float r1 = radius * 0.8;
    float r2 = radius * 2.6;

    vec3 center = sampleHighlightAt(vTexCoord, threshold);

    vec3 ring1 = vec3(0.0);
    ring1 += sampleHighlightAt(vTexCoord + vec2( r1, 0.0) * texel, threshold);
    ring1 += sampleHighlightAt(vTexCoord + vec2(-r1, 0.0) * texel, threshold);
    ring1 += sampleHighlightAt(vTexCoord + vec2(0.0,  r1) * texel, threshold);
    ring1 += sampleHighlightAt(vTexCoord + vec2(0.0, -r1) * texel, threshold);
    ring1 += sampleHighlightAt(vTexCoord + vec2( r1,  r1) * texel, threshold);
    ring1 += sampleHighlightAt(vTexCoord + vec2(-r1,  r1) * texel, threshold);
    ring1 += sampleHighlightAt(vTexCoord + vec2( r1, -r1) * texel, threshold);
    ring1 += sampleHighlightAt(vTexCoord + vec2(-r1, -r1) * texel, threshold);
    ring1 *= 1.0 / 8.0;

    vec3 ring2 = vec3(0.0);
    ring2 += sampleHighlightAt(vTexCoord + vec2( r2, 0.0) * texel, threshold);
    ring2 += sampleHighlightAt(vTexCoord + vec2(-r2, 0.0) * texel, threshold);
    ring2 += sampleHighlightAt(vTexCoord + vec2(0.0,  r2) * texel, threshold);
    ring2 += sampleHighlightAt(vTexCoord + vec2(0.0, -r2) * texel, threshold);
    ring2 += sampleHighlightAt(vTexCoord + vec2( r2,  r2) * texel, threshold);
    ring2 += sampleHighlightAt(vTexCoord + vec2(-r2,  r2) * texel, threshold);
    ring2 += sampleHighlightAt(vTexCoord + vec2( r2, -r2) * texel, threshold);
    ring2 += sampleHighlightAt(vTexCoord + vec2(-r2, -r2) * texel, threshold);
    ring2 *= 1.0 / 8.0;

    // Weighted two-lobe composite
    vec3 halation = center * 0.25 + ring1 * 0.50 + ring2 * 0.25;

    vec3 outLinear = baseLinear + halation * amount;
    FragColor = vec4(LinearToSrgb3(outLinear), 1.0);
}
