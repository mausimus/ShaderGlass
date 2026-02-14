#version 450

#pragma stage vertex
layout(location = 0) in vec4 Position;
layout(location = 1) in vec2 TexCoord;
layout(location = 0) out vec2 vTexCoord;
void main() {
    gl_Position = Position;
    vTexCoord = TexCoord;
}

#pragma stage fragment
layout(location = 0) in vec2 vTexCoord;
layout(location = 0) out vec4 FragColor;
layout(set = 0, binding = 2) uniform sampler2D Source;
layout(set = 0, binding = 3) uniform sampler2D PassFeedback;

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

    FragColor = vec4(smoothLines, smoothConf, curLines, 1.0);
}
