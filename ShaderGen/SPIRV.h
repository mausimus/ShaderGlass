#pragma once

#include <vector>
#include <string>

class SPIRV
{
public:
    static std::pair<std::string, std::string> GenerateHLSL(const std::vector<uint32_t>& bin, bool fragment);
};