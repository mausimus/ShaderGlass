#version 450
// nicelines v11 (2026-02-14)

// === PASS 3: HALATION EXTRACT ===
// Input: masked image from pass 2 (linear)
// Output: extracted highlight mask in linear space

#pragma parameter HALATION_THRESHOLD "HALATION Threshold" 0.79 0.6 0.99 0.01
#pragma parameter HALATION_GROW_RADIUS "HALATION Extract Grow Radius" 1.2 0.0 8.0 0.1
#pragma parameter HALATION_GROW_FILL "HALATION Extract Grow Fill" 0.75 0.0 1.0 0.05

layout(std140, set = 0, binding = 0) uniform UBO {
    mat4 MVP;
    vec4 SourceSize;
    vec4 OriginalSize;
    vec4 OutputSize;
    uint FrameCount;
    float HALATION_THRESHOLD;
    float HALATION_GROW_RADIUS;
    float HALATION_GROW_FILL;
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

vec3 sampleExtract(vec2 uv, float threshold) {
    vec3 c = texture(Source, clamp(uv, vec2(0.0), vec2(1.0))).rgb;
    return extractHighlights(c, threshold);
}

vec3 growHighlights(vec2 uv, float threshold, float radiusPx, float fill) {
    vec3 center = sampleExtract(uv, threshold);
    if(radiusPx <= 0.001) return center;

    vec2 texel = 1.0 / global.OutputSize.xy;
    vec2 d = texel * radiusPx;

    vec2 dirs[8] = vec2[](
        vec2( 1.0,  0.0), vec2(-1.0,  0.0),
        vec2( 0.0,  1.0), vec2( 0.0, -1.0),
        normalize(vec2( 1.0,  1.0)), normalize(vec2(-1.0,  1.0)),
        normalize(vec2( 1.0, -1.0)), normalize(vec2(-1.0, -1.0))
    );

    vec3 maxV = center;
    vec3 sumV = center;

    for (int i = 0; i < 8; i++) {
        vec3 s = sampleExtract(uv + dirs[i] * d, threshold);
        maxV = max(maxV, s);
        sumV += s;
    }

    vec3 avgV = sumV / 9.0;
    return max(maxV, avgV * fill);
}

void main() {
    vec3 grown = growHighlights(
        vTexCoord,
        global.HALATION_THRESHOLD,
        global.HALATION_GROW_RADIUS,
        global.HALATION_GROW_FILL
    );
    FragColor = vec4(grown, 1.0);
}
