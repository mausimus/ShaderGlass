#version 450

#pragma parameter DEBUG_VIEW "DEBUG View (0=Temporal 1=UV 2=Feedback 3=Delta 4=Grid)" 0.0 0.0 4.0 1.0

layout(std140, set = 0, binding = 0) uniform UBO {
    mat4 MVP;
    vec4 SourceSize;
    vec4 OriginalSize;
    vec4 OutputSize;
    uint FrameCount;
    float DEBUG_VIEW;
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
layout(set = 0, binding = 3) uniform sampler2D PassFeedback;

vec3 debugGrid(vec2 uv) {
    vec2 g = abs(fract(uv * 20.0) - 0.5);
    float line = step(g.x, 0.03) + step(g.y, 0.03);
    return mix(vec3(0.10, 0.10, 0.12), vec3(0.20, 0.85, 0.25), clamp(line, 0.0, 1.0));
}

void main() {
    const float TEMP_ALPHA = 0.10;
    const float SWITCH_BIAS = 0.08;

    vec3 cur = texture(Source, vTexCoord).rgb;
    vec3 prev = texture(PassFeedback, vTexCoord).rgb;

    float curLines = cur.r;
    float curConf = cur.g;
    float prevLines = prev.r;
    float prevConf = prev.g;

    bool uninitialized = (prevLines <= 0.0001 && prevConf <= 0.0001);
    if(uninitialized) {
        FragColor = vec4(curLines, curConf, curLines, 1.0);
        return;
    }

    float delta = abs(curLines - prevLines);
    float alpha = TEMP_ALPHA * (0.35 + 0.65 * curConf);

    if(delta > (2.0 / 1024.0) && curConf > prevConf + SWITCH_BIAS) {
        alpha = max(alpha, 0.30);
    }

    float smoothLines = mix(prevLines, curLines, clamp(alpha, 0.0, 1.0));
    float smoothConf = mix(prevConf, curConf, 0.20);

    int dbg = int(global.DEBUG_VIEW + 0.5);
    if(dbg == 1) {
        FragColor = vec4(vTexCoord, 0.0, 1.0);
        return;
    }
    if(dbg == 2) {
        float valid = uninitialized ? 0.0 : 1.0;
        FragColor = vec4(prevLines, prevConf, valid, 1.0);
        return;
    }
    if(dbg == 3) {
        float deltaVis = clamp(delta * 1024.0 * 0.25, 0.0, 1.0);
        FragColor = vec4(deltaVis, curConf, prevConf, 1.0);
        return;
    }
    if(dbg == 4) {
        FragColor = vec4(debugGrid(vTexCoord), 1.0);
        return;
    }

    FragColor = vec4(smoothLines, smoothConf, curLines, 1.0);
}
