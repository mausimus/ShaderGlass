#pragma once

struct ShaderParam
{
    ShaderParam(const char* name, int buffer, int offset, int size, float minValue, float maxValue, float defaultValue, float stepValue = 0.0f, const char* description = "") :
        name {name}, buffer {buffer}, offset {offset}, size {size}, minValue {minValue}, maxValue {maxValue}, defaultValue {defaultValue},
        currentValue {defaultValue}, stepValue {stepValue}, description {description}
    { }

    std::string name;
    int         buffer;
    int         size;
    int         offset;
    float       minValue;
    float       maxValue;
    float       currentValue;
    float       defaultValue;
    float       stepValue;
    std::string description;
};

struct ShaderSampler
{
    ShaderSampler(const char* name, int binding) : name {name}, binding {binding} { }
    std::string name;
    int         binding;
};

class ShaderDef
{
public:
    ShaderDef() :
        Params {}, Samplers {}, VertexSource {}, FragmentSource {}, Name {}, VertexByteCode {}, FragmentByteCode {}, VertexLength {},
        FragmentLength {}
    { }

    std::vector<ShaderParam> Params;
    std::vector<ShaderSampler> Samplers;
    std::map<std::string, std::string> PresetParams;
    const char* VertexSource;
    const char* FragmentSource;
    const char* Name;
    const BYTE* VertexByteCode;
    const BYTE* FragmentByteCode;
    SIZE_T VertexLength;
    SIZE_T FragmentLength;

    size_t ParamsSize(int buffer)
    {
        int maxLen = 0;
        for(const auto& p : Params)
        {
            if(p.buffer == buffer)
            {
                if(p.offset + p.size > maxLen)
                    maxLen = p.offset + p.size;
            }
        }
        return maxLen;
    }

    ShaderDef& Param(const std::string& presetKey, const std::string& presetValue)
    {
        PresetParams.insert(std::make_pair(presetKey, presetValue));
        return *this;
    }
};