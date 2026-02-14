#version 450

#pragma parameter DEBUG_VIEW "DEBUG View (0=Detector 1=UV 2=SourceSize 3=Grid 4=FramePulse)" 0.0 0.0 4.0 1.0

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

vec3 debugGrid(vec2 uv) {
    vec2 g = abs(fract(uv * 20.0) - 0.5);
    float line = step(g.x, 0.03) + step(g.y, 0.03);
    return mix(vec3(0.10, 0.10, 0.12), vec3(0.95, 0.30, 0.20), clamp(line, 0.0, 1.0));
}

float scoreCandidate(float srcH, float cand) {
    float scale = srcH / cand;
    float fracErr = abs(scale - round(scale));
    float penalty = (scale < 1.0) ? (1.0 - scale) * 0.25 : 0.0;
    return fracErr + penalty;
}

void main() {
    vec2 srcSize = vec2(textureSize(Source, 0));
    float srcH = srcSize.y;

    const int N = 12;
    const float CANDS[N] = float[](
        192.0, 200.0, 224.0, 240.0, 256.0, 288.0,
        320.0, 360.0, 384.0, 400.0, 448.0, 480.0
    );

    float bestCand = CANDS[0];
    float bestScore = 1e9;
    for(int i = 0; i < N; i++) {
        float s = scoreCandidate(srcH, CANDS[i]);
        if(s < bestScore) {
            bestScore = s;
            bestCand = CANDS[i];
        }
    }

    float conf = 1.0 - clamp(bestScore * 10.0, 0.0, 1.0);
    float linesNorm = bestCand / 1024.0;

    int dbg = int(global.DEBUG_VIEW + 0.5);
    if(dbg == 1) {
        FragColor = vec4(vTexCoord, 0.0, 1.0);
        return;
    }
    if(dbg == 2) {
        FragColor = vec4(
            clamp(srcSize.x / 4096.0, 0.0, 1.0),
            clamp(srcSize.y / 2160.0, 0.0, 1.0),
            fract(srcSize.y / 100.0),
            1.0
        );
        return;
    }
    if(dbg == 3) {
        FragColor = vec4(debugGrid(vTexCoord), 1.0);
        return;
    }
    if(dbg == 4) {
        float pulse = 0.2 + 0.8 * abs(sin(float(global.FrameCount) * 0.15));
        FragColor = vec4(pulse, pulse * 0.25, 1.0 - pulse, 1.0);
        return;
    }

    FragColor = vec4(linesNorm, conf, linesNorm, 1.0);
}
