#version 450

#pragma stage vertex
layout(location = 0) in vec4 Position;
layout(location = 1) in vec2 TexCoord;
layout(location = 0) out vec2 vTexCoord;
void main() {
    gl_Position = vec4(Position.xy * 2.0 - 1.0, Position.z, Position.w);
    vTexCoord = TexCoord;
}

#pragma stage fragment
layout(location = 0) in vec2 vTexCoord;
layout(location = 0) out vec4 FragColor;
layout(set = 0, binding = 2) uniform sampler2D Source;

float scoreCandidate(float srcH, float cand) {
    float scale = srcH / cand;
    float fracErr = abs(scale - round(scale));
    float penalty = (scale < 1.0) ? (1.0 - scale) * 0.25 : 0.0;
    return fracErr + penalty;
}

void main() {
    float srcH = float(textureSize(Source, 0).y);

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
    FragColor = vec4(linesNorm, conf, linesNorm, 1.0);
}
