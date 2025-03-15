#pragma once

class GLSL
{
public:
    static std::vector<uint32_t> GenerateSPIRV(const char* source, bool fragment, std::ostream& log, bool& warn);
};