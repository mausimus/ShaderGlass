#version 450
// nicelines v9 (2026-02-14)

// === PASS 3: HALATION EXTRACT ===
// Input: masked image from pass 2 (linear)
// Output: extracted highlight mask in linear space

#pragma parameter HALATION_THRESHOLD "HALATION Threshold" 0.79 0.6 0.99 0.01

layout(std140, set = 0, binding = 0) uniform UBO {
    mat4 MVP;
    vec4 SourceSize;
    vec4 OriginalSize;
    vec4 OutputSize;
    uint FrameCount;
    float HALATION_THRESHOLD;
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

void main() {
    vec3 baseLinear = texture(Source, vTexCoord).rgb;
    vec3 extracted = extractHighlights(baseLinear, global.HALATION_THRESHOLD);
    FragColor = vec4(extracted, 1.0);
}
