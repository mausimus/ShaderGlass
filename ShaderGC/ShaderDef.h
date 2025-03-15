#pragma once

struct ShaderParam
{
    ShaderParam(const char* name, int buffer, int offset, int size, float minValue, float maxValue, float defaultValue, float stepValue = 0.0f, const char* description = "") :
        name {name}, buffer {buffer}, offset {offset}, size {size}, minValue {minValue}, maxValue {maxValue}, defaultValue {defaultValue}, currentValue {defaultValue},
        stepValue {stepValue}, description {description}
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

struct ParamOverride
{
    ParamOverride(const char* name, float value) : name {name}, value {value} { }

    std::string name;
    float       value;
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
        Params {}, Samplers {}, Name {}, VertexSource {}, FragmentSource {}, VertexByteCode {}, FragmentByteCode {}, VertexLength {}, FragmentLength {}, Format {}, Dynamic {false}
    { }

    std::vector<ShaderParam>           Params;
    std::vector<ShaderSampler>         Samplers;
    std::map<std::string, std::string> PresetParams;
    std::string                        Name;
    const char*                        VertexSource;
    const char*                        FragmentSource;
    const uint8_t*                     VertexByteCode;
    const uint8_t*                     FragmentByteCode;
    size_t                             VertexLength;
    size_t                             FragmentLength;
    char*                              Format;
    bool                               Dynamic;

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

    virtual ~ShaderDef()
    {
        if(Dynamic)
        {
            if(VertexByteCode)
                free((void*)VertexByteCode);
            if(FragmentByteCode)
                free((void*)FragmentByteCode);
        }
    }
};