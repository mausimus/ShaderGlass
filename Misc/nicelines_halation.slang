#version 450
// nicelines v11 (2026-02-14)

// === PASS 4: HALATION BLUR + FINAL OUTPUT ===
// Input A (Source): extracted highlights from pass 3 (linear)
// Input B (Mask):   base masked image from pass 2 (linear, via alias)
// Output: final image in sRGB

#pragma parameter COMPARE "COMPARE (Off / On)" 0.0 0.0 1.0 1.0

layout(std140, set = 0, binding = 0) uniform UBO {
    mat4 MVP;
    vec4 SourceSize;
    vec4 OriginalSize;
    vec4 OutputSize;
    uint FrameCount;
    float COMPARE;
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

void main() {
    vec3 baseLinear = texture(Mask, vTexCoord).rgb;

    if(global.COMPARE > 0.5 && vTexCoord.x < 0.5) {
        FragColor = vec4(LinearToSrgb3(baseLinear), 1.0);
        return;
    }

    const float amount = 0.48;
    const float radius = 12.0;

    if(amount <= 0.0001 || radius <= 0.0001) {
        FragColor = vec4(LinearToSrgb3(baseLinear), 1.0);
        return;
    }

    vec2 texel = 1.0 / global.OutputSize.xy;

    vec3 blurTight = blurHighlightsGaussian(vTexCoord, radius, texel);
    vec3 blurWide = blurHighlightsGaussian(vTexCoord, radius * 1.8, texel);
    vec3 halation = blurTight * 0.7 + blurWide * 0.3;

    vec3 outLinear = baseLinear + halation * amount;
    FragColor = vec4(LinearToSrgb3(outLinear), 1.0);
}
