#pragma once

class TextureDef
{
public:
    TextureDef() : Data {}, DataLength {}, PresetParams {}, Dynamic {false} { }

    std::string                        Name;
    const uint8_t*                           Data;
    int                                DataLength;
    bool                               Dynamic;
    std::map<std::string, std::string> PresetParams;

    TextureDef& Param(const std::string& presetKey, const std::string& presetValue)
    {
        PresetParams.insert(std::make_pair(presetKey, presetValue));
        return *this;
    }

    virtual ~TextureDef()
    {
        if(Dynamic)
        {
            if(Data)
            {
                free((void *)Data);
            }
        }
    }
};