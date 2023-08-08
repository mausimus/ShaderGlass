#pragma once

class TextureDef
{
public:
    TextureDef() : Data {}, DataLength {}, PresetParams {} { }

    const char*                        Name;
    const BYTE*                        Data;
    int                                DataLength;
    std::map<std::string, std::string> PresetParams;

    TextureDef& Param(const std::string& presetKey, const std::string& presetValue)
    {
        PresetParams.insert(std::make_pair(presetKey, presetValue));
        return *this;
    }
};