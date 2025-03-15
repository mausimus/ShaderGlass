#pragma once

class SPIRV
{
public:
    static std::pair<std::string, std::string> GenerateHLSL(const std::vector<uint32_t>& bin, bool fragment, std::ostream& log, bool& warn);
};