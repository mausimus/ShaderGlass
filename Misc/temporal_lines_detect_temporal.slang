#version 450

#pragma parameter TEMP_ALPHA "DETECT Temporal Alpha" 0.10 0.0 1.0 0.01
#pragma parameter SWITCH_BIAS "DETECT Switch Bias" 0.08 0.0 0.5 0.01

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
layout(set = 0, binding = 2) uniform sampler2D Source;        // pass0 current estimate
layout(set = 0, binding = 3) uniform sampler2D PassFeedback;  // previous pass1

void main() {
    vec3 cur = texture(Source, vTexCoord).rgb;
    vec3 prev = texture(PassFeedback, vTexCoord).rgb;

    float curLines = cur.r;
    float curConf = cur.g;
    float prevLines = prev.r;
    float prevConf = prev.g;

    // Initialize quickly if feedback is empty-ish
    bool uninitialized = (prevLines <= 0.0001 && prevConf <= 0.0001);
    if(uninitialized) {
        FragColor = vec4(curLines, curConf, curLines, 1.0);
        return;
    }

    float delta = abs(curLines - prevLines);
    float alpha = TEMP_ALPHA * (0.35 + 0.65 * curConf);

    // Allow faster switches only when confidence improves significantly.
    if(delta > (2.0 / 1024.0) && curConf > prevConf + SWITCH_BIAS) {
        alpha = max(alpha, 0.30);
    }

    float smoothLines = mix(prevLines, curLines, clamp(alpha, 0.0, 1.0));
    float smoothConf = mix(prevConf, curConf, 0.20);

    // Visualization:
    // R: smoothed estimate, G: smoothed confidence, B: current raw estimate
    FragColor = vec4(smoothLines, smoothConf, curLines, 1.0);
}
