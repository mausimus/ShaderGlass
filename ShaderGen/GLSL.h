#pragma once

#include <vector>

typedef struct SpirVShader
{
    std::vector<uint32_t> vertexData;
    std::vector<uint32_t> fragmentData;
} SpirVShader;

class GLSL
{
public:
	static SpirVShader GenerateSPIRV(const char* vertexSource, const char* fragmentSource);
    static std::vector<uint32_t> GenerateSPIRV(const char* source, bool fragment);
};