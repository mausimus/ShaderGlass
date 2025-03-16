// ShaderGC.cpp : Defines the functions for the static library.
//

#include "pch.h"
#include "framework.h"
#include "ShaderGC.h"

#include "GLSL.h"
#include "HLSL.h"
#include "SPIRV.h"

#include "json.hpp"

using namespace std;
using namespace nlohmann;

static char* CopyString(const std::string& s)
{
    auto copy = new char[s.size() + 1];
    strcpy_s(copy, s.size() + 1, s.c_str());
    return copy;
}

static uint8_t* CopyVector(const std::vector<uint8_t>& d)
{
    auto copy = new uint8_t[d.size()];
    memcpy(copy, d.data(), d.size());
    return copy;
}

ShaderDef ShaderGC::CompileSourceShader(SourceShaderDef& def, ostream& log, bool& warn)
{
    // convert GLSL to SPIRV
    auto vertexSPIRV   = GLSL::GenerateSPIRV(def.vertexSource.c_str(), false, log, warn);
    auto fragmentSPIRV = GLSL::GenerateSPIRV(def.fragmentSource.c_str(), true, log, warn);

    // convert SPIRV to HLSL and reflect
    auto vertexHLSL   = SPIRV::GenerateHLSL(vertexSPIRV, false, log, warn);
    auto fragmentHLSL = SPIRV::GenerateHLSL(fragmentSPIRV, true, log, warn);

    // compile HLSL to DXBC
    auto vertexDXBC   = HLSL::CompileHLSL(vertexHLSL.first.c_str(), (int)vertexHLSL.first.size(), "vs_5_0", log, warn);
    auto fragmentDXBC = HLSL::CompileHLSL(fragmentHLSL.first.c_str(), (int)fragmentHLSL.first.size(), "ps_5_0", log, warn);

    // map declared to reflected parameters
    std::vector<SourceShaderSampler> textures;
    def.params = LookupParams(def.params, textures, fragmentHLSL.second);

    ShaderDef sd;
    sd.Format           = CopyString(def.format);
    sd.VertexSource     = nullptr;
    sd.VertexByteCode   = CopyVector(vertexDXBC);
    sd.VertexLength     = vertexDXBC.size();
    sd.FragmentSource   = nullptr;
    sd.FragmentByteCode = CopyVector(fragmentDXBC);
    sd.FragmentLength   = fragmentDXBC.size();
    sd.Name             = def.input.filename().string();

    for(const auto& p : def.params)
    {
        sd.Params.push_back(ShaderParam(CopyString(p.name), p.buffer, p.offset, p.size, p.min, p.max, p.def, p.step, CopyString(p.desc)));
    }

    for(const auto& t : textures)
    {
        sd.Samplers.push_back(ShaderSampler(CopyString(t.name), t.binding));
    }

    return sd;
}

PresetDef* ShaderGC::CompileShader(std::filesystem::path source, ostream& log, bool& warn)
{
    SourceShaderDef def(source, SourceShaderInfo());
    ProcessSourceShader(def, log, warn);
    auto shaderDef = CompileSourceShader(def, log, warn);

    // dummy preset
    PresetDef* pdef = new PresetDef();
    pdef->Name      = source.filename().string();
    pdef->Category  = "Imported";
    pdef->ShaderDefs.push_back(shaderDef);

    return pdef;
}

vector<string> ShaderGC::LoadSource(const filesystem::path& input, bool followIncludes)
{
    vector<string> lines;

    fstream infile(input);
    string  line;
    while(getline(infile, line))
    {
        if(followIncludes && line.starts_with("#include"))
        {
            istringstream iss(line);
            string        incDirective, incFile;
            iss >> incDirective;
            iss >> quoted(incFile);
            filesystem::path includePath(input);
            includePath.remove_filename();
            includePath /= filesystem::path(incFile);
            const auto& includeLines = LoadSource(includePath.lexically_normal(), true);
            lines.insert(lines.end(), includeLines.begin(), includeLines.end());
        }
        else
            lines.push_back(line);
    }
    infile.close();

    return lines;
}

void ShaderGC::ProcessSourceShader(SourceShaderDef& def, ostream& log, bool& warn)
{
    ostringstream vertexSource;
    ostringstream fragmentSource;

    bool        isVertex = true, isFragment = true;
    const auto& source    = LoadSource(def.input, true);
    bool        inComment = false;
    for(const auto& line : source)
    {
        auto trimLine = trim(line);
        if(line.starts_with("#pragma parameter"))
        {
            // de-duping as workaround for repeated includes
            auto param = SourceShaderParam(line, 1, 0);
            bool dupe  = false;
            for(const auto& p : def.params)
            {
                if(p.name == param.name)
                {
                    dupe = true;
                    break;
                }
            }
            if(!dupe)
                def.params.push_back(param);
        }
        else if(line.starts_with("#pragma stage vertex"))
        {
            isFragment = false;
            isVertex   = true;
            inComment  = false;
        }
        else if(line.starts_with("#pragma stage fragment"))
        {
            isFragment = true;
            isVertex   = false;
            inComment  = false;
        }
        else if(trimLine.starts_with("#pragma format"))
        {
            if(trimLine.ends_with("R8G8B8A8_UNORM"))
            {
                def.format = "R8G8B8A8_UNORM";
            }
            else if(trimLine.ends_with("R8G8B8A8_SRGB"))
            {
                def.format = "R8G8B8A8_SRGB";
            }
            else if(trimLine.ends_with("R32G32B32A32_SFLOAT"))
            {
                def.format = "R32G32B32A32_SFLOAT";
            }
            else if(trimLine.ends_with("R16G16B16A16_SFLOAT"))
            {
                def.format = "R16G16B16A16_SFLOAT";
            }
            else
            {
                throw std::runtime_error("Sorry, unsupported format " + trimLine.substr(15));
            }
        }
        else if(trimLine.starts_with("//"))
        {
            if(trimLine.ends_with("*/"))
            {
                //def.comments.push_back(trimLine.substr(0, trimLine.length() - 2));
                inComment = false;
            }
            else
            {
                def.comments.push_back(trimLine);
            }
        }
        else if(trimLine.starts_with("/*"))
        {
            if(trimLine.ends_with("*/"))
            {
                def.comments.push_back(trimLine.substr(2, trimLine.length() - 4));
            }
            else if(trimLine.find_first_of("*/") == string::npos)
            {
                def.comments.push_back(trimLine.substr(2));
                inComment = true;
            }
            else
            {
                // comment within line
                if(isFragment)
                    fragmentSource << line << endl;
                if(isVertex)
                    vertexSource << line << endl;
            }
        }
        else if(inComment && trimLine.ends_with("*/"))
        {
            def.comments.push_back(trimLine.substr(0, trimLine.length() - 2));
            inComment = false;
        }
        else if(inComment && trimLine.starts_with("*/"))
        {
            inComment = false;
        }
        else if(inComment)
        {
            def.comments.push_back(trimLine);
        }
        else
        {
            if(isFragment)
                fragmentSource << line << endl;
            if(isVertex)
                vertexSource << line << endl;
        }
    }

    def.fragmentSource = fragmentSource.str();
    def.vertexSource   = vertexSource.str();

    // built-in parameters
    def.params.push_back(SourceShaderParam("MVP", 16, 0));
    def.params.push_back(SourceShaderParam("SourceSize", 4, 0));
    def.params.push_back(SourceShaderParam("OriginalSize", 4, 0));
    def.params.push_back(SourceShaderParam("OutputSize", 4, 0));
    def.params.push_back(SourceShaderParam("FrameCount", 1, 0));
}

int GetSize(const std::string& mtype)
{
    if(mtype == "float" || mtype == "uint" || mtype == "int")
    {
        return 4;
    }
    else if(mtype == "vec2")
    {
        return 8;
    }
    else if(mtype == "vec3")
    {
        return 12;
    }
    else if(mtype == "vec4")
    {
        return 16;
    }
    else if(mtype == "mat4")
    {
        return 64;
    }
    else
    {
        throw std::runtime_error("Unknown type");
    }
}

void AddParams(vector<SourceShaderParam>& actualParams, const vector<SourceShaderParam>& declaredParams, json type, int buffer)
{
    auto members = type.at("members");
    for(json::iterator mi = members.begin(); mi != members.end(); ++mi)
    {
        auto member  = *mi;
        auto mname   = (string)member.at("name");
        auto moffset = (int)member.at("offset");
        auto mtype   = (string)member.at("type");

        bool paramFound = false;
        for(auto& p : declaredParams)
        {
            if(p.name == mname)
            {
                SourceShaderParam actualParam(p);
                actualParam.i      = 0;
                actualParam.buffer = buffer;
                actualParam.offset = moffset;
                actualParam.size   = GetSize(mtype);
                actualParams.emplace_back(actualParam);
                paramFound = true;
            }
        }

        if(!paramFound)
        {
            // alias/built-in param?
            SourceShaderParam newParam(mname, GetSize(mtype), 0);
            newParam.offset = moffset;
            newParam.buffer = buffer;
            newParam.i      = 0;
            actualParams.emplace_back(newParam);
        }
    }
}

std::vector<SourceShaderParam> ShaderGC::LookupParams(const std::vector<SourceShaderParam>& declaredParams, vector<SourceShaderSampler>& textures, const std::string& metadata)
{
    vector<SourceShaderParam> actualParams;

    auto j     = json::parse(metadata);
    auto types = j["types"];
    auto ubos  = j["ubos"];
    for(json::iterator it = ubos.begin(); it != ubos.end(); ++it)
    {
        auto ubo     = *it;
        auto typeNo  = ubo.at("type");
        auto binding = (int)ubo.at("binding");
        auto type    = types.at((string)typeNo);
        AddParams(actualParams, declaredParams, type, binding);
    }

    auto pcs = j["push_constants"];
    int  ci  = -1;
    for(json::iterator it = pcs.begin(); it != pcs.end(); ++it)
    {
        auto pc     = *it;
        auto typeNo = pc.at("type");
        auto type   = types.at((string)typeNo);
        AddParams(actualParams, declaredParams, type, ci--);
    }

    auto txs = j["textures"];
    for(json::iterator it = txs.begin(); it != txs.end(); ++it)
    {
        auto tx = *it;
        textures.push_back(SourceShaderSampler((string)tx.at("name"), (int)tx.at("binding")));
    }

    return actualParams;
}

pair<string, string> getKeyValue(string input)
{
    auto key   = trim(input.substr(0, input.find("=")));
    auto value = trim(input.substr(input.find("=") + 1));
    if(value.find("\"") != string::npos)
    {
        value = value.substr(0, value.find("\"")); // strip anything beyond quote (comment)
    }
    return make_pair(key, value);
}

string getValue(const string& key, int shaderNo, const map<string, string>& keyValues, unordered_set<string>& seenKeys)
{
    stringstream ss;
    ss << key;
    if(shaderNo >= 0)
    {
        ss << shaderNo;
    }
    if(keyValues.find(ss.str()) != keyValues.end())
    {
        seenKeys.insert(ss.str());
        return keyValues.at(ss.str());
    }
    return string();
}

string getValue(const string& key, const string& suffix, const map<string, string>& keyValues, unordered_set<string>& seenKeys)
{
    stringstream ss;
    ss << key << suffix;
    if(keyValues.find(ss.str()) != keyValues.end())
    {
        seenKeys.insert(ss.str());
        return keyValues.at(ss.str());
    }
    return string();
}

filesystem::path getKeyPath(const string& key, int shaderNo, const map<string, filesystem::path>& keyPaths)
{
    stringstream ss;
    ss << key;
    if(shaderNo >= 0)
    {
        ss << shaderNo;
    }
    if(keyPaths.find(ss.str()) != keyPaths.end())
    {
        return keyPaths.at(ss.str());
    }
    return filesystem::path();
}

filesystem::path getKeyPath(const string& key, const string& suffix, const map<string, filesystem::path>& keyPaths)
{
    stringstream ss;
    ss << key << suffix;
    if(keyPaths.find(ss.str()) != keyPaths.end())
    {
        return keyPaths.at(ss.str());
    }
    return filesystem::path();
}

filesystem::path getPath(const string& key, int shaderNo, const map<string, string>& keyValues, const map<string, filesystem::path>& keyPaths, unordered_set<string>& seenKeys)
{
    auto valuePath = filesystem::path(getValue(key, shaderNo, keyValues, seenKeys));
    auto keyPath   = getKeyPath(key, shaderNo, keyPaths);
    return keyPath / valuePath;
}

filesystem::path
getPath(const string& key, const string& suffix, const map<string, string>& keyValues, const map<string, filesystem::path>& keyPaths, unordered_set<string>& seenKeys)
{
    auto valuePath = filesystem::path(getValue(key, suffix, keyValues, seenKeys));
    auto keyPath   = getKeyPath(key, suffix, keyPaths);
    return keyPath / valuePath;
}

void setPresetParam(const string& paramName, SourceShaderDef& def, int i, const map<string, string>& keyValues, unordered_set<string>& seenKeys)
{
    const auto& value = getValue(paramName, i, keyValues, seenKeys);
    if(!value.empty())
        def.presetParams.insert(make_pair(paramName, value));
}

void setPresetParam(const string& paramName, SourceTextureDef& def, const string& suffix, const map<string, string>& keyValues, unordered_set<string>& seenKeys)
{
    const auto& value = getValue(paramName, suffix, keyValues, seenKeys);
    if(!value.empty())
        def.presetParams.insert(make_pair(suffix, value));
}

void setPresetParams(SourceShaderDef& def, int i, const map<string, string>& keyValues, unordered_set<string>& seenKeys)
{
    setPresetParam("filter_linear", def, i, keyValues, seenKeys);
    setPresetParam("float_framebuffer", def, i, keyValues, seenKeys);
    setPresetParam("srgb_framebuffer", def, i, keyValues, seenKeys);
    setPresetParam("scale_type", def, i, keyValues, seenKeys);
    setPresetParam("scale", def, i, keyValues, seenKeys);
    setPresetParam("scale_type_x", def, i, keyValues, seenKeys);
    setPresetParam("scale_x", def, i, keyValues, seenKeys);
    setPresetParam("scale_type_y", def, i, keyValues, seenKeys);
    setPresetParam("scale_y", def, i, keyValues, seenKeys);
    setPresetParam("alias", def, i, keyValues, seenKeys);
    setPresetParam("mipmap_input", def, i, keyValues, seenKeys);
    setPresetParam("frame_count_mod", def, i, keyValues, seenKeys);
    setPresetParam("wrap_mode", def, i, keyValues, seenKeys);
}

void setPresetParams(SourceTextureDef& def, std::string name, const map<string, string>& keyValues, unordered_set<string>& seenKeys)
{
    setPresetParam(name + "_", def, "linear", keyValues, seenKeys);
    setPresetParam(name + "_", def, "wrap_mode", keyValues, seenKeys);
    setPresetParam(name + "_", def, "mipmap", keyValues, seenKeys);
}

void ShaderGC::ParsePreset(const std::filesystem::path& input, std::map<std::string, std::string>& keyValues, std::map<std::string, std::filesystem::path>& valuePaths)
{
    fstream infile(input);
    string  line;
    while(getline(infile, line))
    {
        if(line.starts_with("#reference"))
        {
            istringstream iss(line);
            string        incDirective, incFile;
            iss >> incDirective;
            iss >> quoted(incFile);
            filesystem::path includePath(input);
            includePath.remove_filename();
            includePath /= filesystem::path(incFile);
            ParsePreset(includePath, keyValues, valuePaths);
        }
        else if(line.starts_with("#"))
        {
            continue;
        }
        else
        {
            const auto& kv       = getKeyValue(line);
            keyValues[kv.first]  = kv.second;
            valuePaths[kv.first] = input.parent_path();
        }
    }
    infile.close();
}

PresetDef* ShaderGC::CompilePreset(std::filesystem::path input, ostream& log, bool& warn)
{
    if(_stricmp(input.extension().string().c_str(), ".slang") == 0)
        return CompileShader(input, log, warn);

    SourcePresetDef sp(input, SourceShaderInfo());
    ProcessSourcePreset(sp, log, warn);

    PresetDef* def = new PresetDef();
    def->Name     = input.filename().string();
    def->Category = "Imported";

    for(auto& s : sp.shaders)
    {
        ProcessSourceShader(s, log, warn);
        auto sd = CompileSourceShader(s, log, warn);
        for(auto& pp : s.presetParams)
        {
            sd.Param(pp.first, pp.second);
        }
        def->ShaderDefs.push_back(sd);
    }

    for(auto& t : sp.textures)
    {
        auto td = CompileTexture(t.input, log, warn);
        for(auto& pp : t.presetParams)
        {
            td.Param(pp.first, pp.second);
        }
        def->TextureDefs.push_back(td);
    }

    for(auto& o : sp.overrides)
    {
        def->OverrideParam(CopyString(o.name), o.def);
    }

    return def;
}

void ShaderGC::ProcessSourcePreset(SourcePresetDef& def, std::ostream& log, bool& warn)
{
    map<string, string>           keyValues;
    map<string, filesystem::path> keyPaths;
    unordered_set<string>         seenKeys;

    ParsePreset(def.input, keyValues, keyPaths);

    auto numShaders = atoi(getValue("shaders", -1, keyValues, seenKeys).c_str());
    for(int i = 0; i < numShaders; i++)
    {
        const auto& shaderRelativePath = getPath("shader", i, keyValues, keyPaths, seenKeys);
        auto        shaderFullPath     = shaderRelativePath.lexically_normal();
        shaderFullPath.make_preferred();
        auto sdef = SourceShaderDef(shaderFullPath, SourceShaderInfo());
        setPresetParams(sdef, i, keyValues, seenKeys);
        def.shaders.push_back(sdef);
    }

    auto textureList = getValue("textures", -1, keyValues, seenKeys);
    if(textureList.size())
    {
        textureList += ";";
        size_t pos = 0;
        string textureName;
        while((pos = textureList.find(';')) != string::npos)
        {
            if(textureList == ";")
                break;

            textureName = textureList.substr(0, pos);
            if(textureName.size())
            {
                const auto& textureRelativePath = getPath(textureName, "", keyValues, keyPaths, seenKeys);
                auto        textureFullPath     = textureRelativePath.lexically_normal();
                textureFullPath.make_preferred();

                auto tdef = SourceTextureDef(textureFullPath, SourceShaderInfo());
                tdef.presetParams.insert(make_pair("name", textureName));
                setPresetParams(tdef, textureName, keyValues, seenKeys);
                def.textures.push_back(tdef);
                textureList.erase(0, pos + 1);
            }
        }
    }
    for(const auto& kv : keyValues)
    {
        if(!seenKeys.contains(kv.first) && !kv.first.empty() && !kv.second.empty())
        {
            try
            {
                auto value = stof(kv.second);
                def.overrides.emplace_back(kv.first, value);
            }
            catch(std::invalid_argument& e)
            {
                log << e.what() << " " << kv.first << " = " << kv.second << endl;
                warn = true;
            }
        }
    }
}

TextureDef ShaderGC::CompileTexture(std::filesystem::path source, std::ostream& log, bool& warn)
{
    TextureDef def;

    ifstream inf(source, ios::binary | ios::ate);
    if(inf.bad())
        throw std::runtime_error("Error loading texture " + source.string());

    int size = inf.tellg();
    inf.seekg(0, ios::beg);

    def.Data       = new uint8_t[size];
    def.DataLength = size;
    def.Name       = source.filename().string();
    inf.read((char*)def.Data, size);
    inf.close();

    return def;
}
