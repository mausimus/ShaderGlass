#version 450

// If feedback/history for pass1 exists, this shader blends current gen output
// with previous frame of itself to create temporal persistence.

#pragma stage vertex
layout(std140, set = 0, binding = 0) uniform UBO {
    mat4 MVP;
    vec4 SourceSize;
    vec4 OriginalSize;
    vec4 OutputSize;
    uint FrameCount;
} global;
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
layout(set = 0, binding = 2) uniform sampler2D Source;   // pass0 current
layout(set = 0, binding = 3) uniform sampler2D PassFeedback; // expected previous pass1

void main() {
    vec3 cur = texture(Source, vTexCoord).rgb;
    vec3 prev = texture(PassFeedback, vTexCoord).rgb;

    // Use strong persistence so feedback is obvious.
    vec3 outc = mix(cur, prev, 0.88);
    FragColor = vec4(outc, 1.0);
}
