#pragma once

class HLSL
{
public:
    static std::vector<uint8_t> CompileHLSL(const char* source, int size, const char* profile, std::ostream& log, bool& warn);
};