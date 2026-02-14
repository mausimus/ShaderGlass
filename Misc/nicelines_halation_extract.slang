#version 450
// nicelines v10 (2026-02-14)

// === PASS 3: HALATION EXTRACT ===
// Input: masked image from pass 2 (linear)
// Output: extracted highlight mask in linear space

#pragma parameter HALATION_THRESHOLD "HALATION Threshold" 0.79 0.6 0.99 0.01
#pragma parameter HALATION_PREBLUR "HALATION Extract Pre-Blur" 0.8 0.0 4.0 0.05

layout(std140, set = 0, binding = 0) uniform UBO {
    mat4 MVP;
    vec4 SourceSize;
    vec4 OriginalSize;
    vec4 OutputSize;
    uint FrameCount;
    float HALATION_THRESHOLD;
    float HALATION_PREBLUR;
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

vec3 extractHighlights(vec3 c, float threshold) {
    return max(c - vec3(threshold), vec3(0.0));
}

vec3 samplePreBlurred(vec2 uv, float blurPx) {
    if(blurPx <= 0.001) {
        return texture(Source, uv).rgb;
    }

    vec2 texel = 1.0 / global.OutputSize.xy;
    vec2 o = texel * blurPx;

    vec3 c = texture(Source, uv).rgb * 0.28;
    c += texture(Source, clamp(uv + vec2( o.x, 0.0), vec2(0.0), vec2(1.0))).rgb * 0.14;
    c += texture(Source, clamp(uv + vec2(-o.x, 0.0), vec2(0.0), vec2(1.0))).rgb * 0.14;
    c += texture(Source, clamp(uv + vec2(0.0,  o.y), vec2(0.0), vec2(1.0))).rgb * 0.14;
    c += texture(Source, clamp(uv + vec2(0.0, -o.y), vec2(0.0), vec2(1.0))).rgb * 0.14;
    c += texture(Source, clamp(uv + vec2( o.x,  o.y), vec2(0.0), vec2(1.0))).rgb * 0.04;
    c += texture(Source, clamp(uv + vec2(-o.x,  o.y), vec2(0.0), vec2(1.0))).rgb * 0.04;
    c += texture(Source, clamp(uv + vec2( o.x, -o.y), vec2(0.0), vec2(1.0))).rgb * 0.04;
    c += texture(Source, clamp(uv + vec2(-o.x, -o.y), vec2(0.0), vec2(1.0))).rgb * 0.04;
    return c;
}

void main() {
    vec3 baseLinear = samplePreBlurred(vTexCoord, global.HALATION_PREBLUR);
    vec3 extracted = extractHighlights(baseLinear, global.HALATION_THRESHOLD);
    FragColor = vec4(extracted, 1.0);
}
