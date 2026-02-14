#version 450

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

void main() {
    float t = float(global.FrameCount) * 0.04;
    float pulse = 0.5 + 0.5 * sin((vTexCoord.x * 40.0) + t);
    vec3 color = vec3(pulse, 0.2 + 0.8 * pulse, 1.0 - pulse);
    FragColor = vec4(color, 1.0);
}
