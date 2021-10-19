/*
ShaderGen: slangp shader converter for ShaderGlass
https://github.com/rohatsu/ShaderGlass
GNU General Public License v3.0
*/

#include "ShaderGen.h"

filesystem::path startupPath;
filesystem::path listPath(_outputPath);
vector<string>   shaderList;

static inline void ltrim(std::string& s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) { return !std::isspace(ch) && ch != '\"'; }));
}

static inline void rtrim(std::string& s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) { return !std::isspace(ch) && ch != '\"'; }).base(), s.end());
}

static inline string trim(std::string s)
{
    ltrim(s);
    rtrim(s);
    return s;
}

std::string exec(const char* cmd)
{
    std::array<char, 128> buffer;
    std::string           result;
    cout << cmd << endl;
    std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(cmd, "r"), _pclose);
    if(!pipe)
    {
        throw std::runtime_error("popen() failed!");
    }
    while(fgets(buffer.data(), static_cast<int>(buffer.size()), pipe.get()) != nullptr)
    {
        result += buffer.data();
    }
    return result;
}

void saveSource(const filesystem::path& fileName, const vector<string>& source)
{
    ofstream outfile(fileName);
    for(const auto& s : source)
    {
        outfile << s << endl;
    }
    outfile.close();
}

void saveSource(const filesystem::path& fileName, const string& source)
{
    ofstream outfile(fileName);
    outfile << source << endl;
    outfile.close();
}

filesystem::path glsl(const filesystem::path& shaderPath, const string& stage, const vector<string> source)
{
    filesystem::path input = shaderPath;
    input.replace_extension("." + stage + ".glsl");
    filesystem::path output = shaderPath;
    output.replace_extension("." + stage + ".spv");

    saveSource(input, source);

    stringstream cmd;
    cmd << "\"" << _glslPath << "\" "
        << "-V -S " << stage << " -o " << output.string() << " " << input.string();
    exec(cmd.str().c_str());
    return output;
}

pair<string, string> spirv(const filesystem::path& input)
{
    stringstream cmd1, cmd2;
    cmd1 << "\"" << _spirvPath << "\" "
         << " --hlsl --shader-model 50 " << input.string();
    const auto& code = exec(cmd1.str().c_str());
    cmd2 << "\"" << _spirvPath << "\" " << input.string() << " --reflect";
    const auto& metadata = exec(cmd2.str().c_str());
    return make_pair(code, metadata);
}

string fxc(const filesystem::path& shaderPath, const string& profile, const string& source)
{
    filesystem::path input = shaderPath;
    input.replace_extension("." + profile + ".hlsl");
    filesystem::path output = shaderPath;
    output.replace_extension("." + profile + ".h");

    saveSource(input, source);

    stringstream cmd;
    cmd << "\"" << _fxcPath << "\" "
        << " /O3 /E main /T " << profile << " /Fh " << output.string() << " " << input.string();
    exec(cmd.str().c_str());

    fstream      infile(output.string());
    stringstream outs;
    string       line;
    bool         active = false;
    while(getline(infile, line))
    {
        if(line.starts_with("const BYTE g_main[] ="))
        {
            active = true;
        }
        else if(active)
        {
            if(line.starts_with("};"))
            {
                outs << "};" << endl;
                break;
            }
            outs << line << endl;
        }
    }
    return outs.str();
}

string splitCode(const string& input)
{
    stringstream split;
    split << "R\"(\n";
    for(const auto& c : input)
    {
        if(c == '\r')
            continue;
        if(c == '\n')
            split << "\n";
        else if(c == '\"')
            split << "\"";
        else
            split << c;
    }
    split << ")\"";
    return split.str();
}

int getSize(const std::string& mtype)
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
        throw exception("Unknown type");
    }
}

void mapType(vector<ShaderParam>& params, json type, int buffer)
{
    auto members = type.at("members");
    for(json::iterator mi = members.begin(); mi != members.end(); ++mi)
    {
        auto member  = *mi;
        auto mname   = (string)member.at("name");
        auto moffset = (int)member.at("offset");
        auto mtype   = (string)member.at("type");

        bool paramFound = false;
        for(auto& p : params)
        {
            if(p.name == mname)
            {
                if(paramFound)
                {
                    // duplicate param
                    ShaderParam p2(p);
                    p2.i      = 0;
                    p2.buffer = buffer;
                    p2.offset = moffset;
                    p2.size   = getSize(mtype);
                    params.emplace_back(p2);
                    break;
                }
                else
                {
                    paramFound = true;

                    p.i      = 0;
                    p.buffer = buffer;
                    p.offset = moffset;
                    p.size   = getSize(mtype);
                }
            }
        }

        if(!paramFound)
        {
            // alias/built-in param?
            ShaderParam newParam(mname, getSize(mtype));
            newParam.offset = moffset;
            newParam.buffer = buffer;
            newParam.i      = 0;
            params.emplace_back(newParam);
        }
    }
}

void mapMetadata(vector<ShaderParam>& params, vector<ShaderSampler>& textures, const string& metadata)
{
    auto j     = json::parse(metadata);
    auto types = j["types"];
    auto ubos  = j["ubos"];
    for(json::iterator it = ubos.begin(); it != ubos.end(); ++it)
    {
        auto ubo     = *it;
        auto typeNo  = ubo.at("type");
        auto binding = (int)ubo.at("binding");
        auto type    = types.at((string)typeNo);
        mapType(params, type, binding);
    }

    auto pcs = j["push_constants"];
    int  ci  = -1;
    for(json::iterator it = pcs.begin(); it != pcs.end(); ++it)
    {
        auto pc     = *it;
        auto typeNo = pc.at("type");
        auto type   = types.at((string)typeNo);
        mapType(params, type, ci--);
    }

    auto txs = j["textures"];
    for(json::iterator it = txs.begin(); it != txs.end(); ++it)
    {
        auto tx = *it;
        textures.push_back(ShaderSampler((string)tx.at("name"), (int)tx.at("binding")));
    }
}

void updateShaderList(const ShaderInfo& shaderInfo)
{
    ostringstream oss;
    oss << "#include \"" << shaderInfo.relativePath.string() << "\"";
    const auto& shaderInclude = oss.str();
    if(find(shaderList.begin(), shaderList.end(), shaderInclude) == shaderList.end())
    {
        auto insertSpot = find(shaderList.begin(), shaderList.end(), "// %SHADER_INCLUDE%");
        shaderList.insert(insertSpot, shaderInclude);
        saveSource(listPath, shaderList);
    }
}

void updateTextureList(const ShaderInfo& textureInfo)
{
    ostringstream oss;
    oss << "#include \"" << textureInfo.relativePath.string() << "\"";
    const auto& textureInclude = oss.str();
    if(find(shaderList.begin(), shaderList.end(), textureInclude) == shaderList.end())
    {
        auto insertSpot = find(shaderList.begin(), shaderList.end(), "// %TEXTURE_INCLUDE%");
        shaderList.insert(insertSpot, textureInclude);
        saveSource(listPath, shaderList);
    }
}

void updatePresetList(const ShaderInfo& shaderInfo)
{
    ostringstream oss;
    oss << "#include \"" << shaderInfo.relativePath.string() << "\"";
    const auto& presetInclude = oss.str();

    ostringstream oss2;
    oss2 << "new " << shaderInfo.className << "PresetDef(),";
    const auto& presetClass = oss2.str();

    bool updated = false;

    if(find(shaderList.begin(), shaderList.end(), presetInclude) == shaderList.end())
    {
        auto insertSpot = find(shaderList.begin(), shaderList.end(), "// %PRESET_INCLUDE%");
        shaderList.insert(insertSpot, presetInclude);
        updated = true;
    }

    if(find(shaderList.begin(), shaderList.end(), presetClass) == shaderList.end())
    {
        auto insertSpot = find(shaderList.begin(), shaderList.end(), "// %PRESET_CLASS%");
        shaderList.insert(insertSpot, presetClass);
        updated = true;
    }

    if(updated)
        saveSource(listPath, shaderList);
}

void populateShaderTemplate(ShaderDef def)
{
    const auto& info = def.info;

    fstream           infile(startupPath / filesystem::path("Shader.template"));
    std::stringstream buffer;
    buffer << infile.rdbuf();
    auto bufferString = buffer.str();
    replace(bufferString, "%LIB_NAME%", _libName);
    replace(bufferString, "%CLASS_NAME%", info.className);    
    replace(bufferString, "%SHADER_NAME%", info.shaderName);
    replace(bufferString, "%SHADER_CATEGORY%", info.category);
    replace(bufferString, "%VERTEX_SOURCE%", splitCode(def.vertexSource));
    replace(bufferString, "%FRAGMENT_SOURCE%", splitCode(def.fragmentSource));
    replace(bufferString, "%VERTEX_BYTECODE%", def.vertexByteCode);
    replace(bufferString, "%FRAGMENT_BYTECODE%", def.fragmentByteCode);

    if(def.fragmentByteCode.empty() || def.vertexByteCode.empty())
    {
        throw exception("Shader compilation failed");
    }

    // built-in parameters
    def.params.push_back(ShaderParam("MVP", 16));
    def.params.push_back(ShaderParam("SourceSize", 4));
    def.params.push_back(ShaderParam("OriginalSize", 4));
    def.params.push_back(ShaderParam("OutputSize", 4));
    def.params.push_back(ShaderParam("FrameCount", 1));

    std::vector<ShaderSampler> textures;
    mapMetadata(def.params, textures, def.fragmentMetadata);

    ofstream          outfile(info.outputPath);
    std::stringstream iss(bufferString);
    while(iss.good())
    {
        std::string line;
        getline(iss, line, '\n');
        if(line == "\"\"")
            continue;
        if(line.starts_with("%PARAM"))
        {
            replace(line, "%PARAM%", "");
            for(const auto& p : def.params)
            {
                if(p.i != -1)
                {
                    string paramLine(line);
                    replace(paramLine, "%PARAM_NAME%", p.name);
                    replace(paramLine, "%PARAM_BUFFER%", to_string(p.buffer));
                    replace(paramLine, "%PARAM_SIZE%", to_string(p.size));
                    replace(paramLine, "%PARAM_OFFSET%", to_string(p.offset));
                    replace(paramLine, "%PARAM_MIN%", to_string(p.min));
                    replace(paramLine, "%PARAM_MAX%", to_string(p.max));
                    replace(paramLine, "%PARAM_DEF%", to_string(p.def));
                    replace(paramLine, "%PARAM_STEP%", to_string(p.step));
                    replace(paramLine, "%PARAM_DESC%", p.desc);
                    outfile << paramLine << endl;
                }
            }
        }
        else if(line.starts_with("%TEXTURE"))
        {
            replace(line, "%TEXTURE%", "");
            for(const auto& t : textures)
            {
                string textureLine(line);
                replace(textureLine, "%TEXTURE_NAME%", t.name);
                replace(textureLine, "%TEXTURE_BINDING%", to_string(t.binding));
                outfile << textureLine << endl;
            }
        }
        else if(line.starts_with("%HEADER"))
        {
            outfile << "ShaderGlass shader " << info.category << "\\" << info.shaderName << " imported from " << _libName << ":" << endl;
            outfile << "https://github.com/libretro/slang-shaders/blob/master/" << def.input.generic_string() << endl;
            outfile << "See original file for full credits and usage license with excerpts below. " << endl;
            outfile << "This file is auto-generated, do not modify directly." << endl;
            if (def.comments.size())
            {
                outfile << endl;
                for(const auto& c: def.comments)
                    outfile << c << endl;
            }
            outfile << endl;
        }
        else
            outfile << line << endl;
    }
    outfile.close();
    cout << "Generated ShaderDef " << info.outputPath << endl;
}

void populateTextureTemplate(TextureDef def)
{
    const auto& info = def.info;

    fstream           infile(startupPath / filesystem::path("Texture.template"));
    std::stringstream buffer;
    buffer << infile.rdbuf();
    auto bufferString = buffer.str();
    replace(bufferString, "%LIB_NAME%", _libName);
    replace(bufferString, "%CLASS_NAME%", info.className);
    replace(bufferString, "%TEXTURE_DATA%", def.data);

    ofstream          outfile(info.outputPath);
    std::stringstream iss(bufferString);
    while(iss.good())
    {
        std::string line;
        getline(iss, line, '\n');
        if(line == "\"\"")
            continue;
        if(line.starts_with("%HEADER"))
        {
            outfile << "ShaderGlass texture " << info.category << " / " << info.shaderName << " imported from " << _libName << ":" << endl;
            outfile << "https://github.com/libretro/slang-shaders/blob/master/" << def.input.generic_string() << endl;
            outfile << "See original file for credits and usage license. " << endl;
            outfile << "This file is auto-generated, do not modify directly." << endl;
        }
        else
            outfile << line << endl;
    }
    outfile.close();
    cout << "Generated TextureDef " << info.outputPath << endl;
}

void populatePresetTemplate(const filesystem::path& input, const vector<ShaderDef>& shaders, const vector<TextureDef>& textures)
{
    const auto& info = getShaderInfo(input, "PresetDef");

    fstream           infile(startupPath / filesystem::path("Preset.template"));
    std::stringstream buffer;
    buffer << infile.rdbuf();
    auto bufferString = buffer.str();
    replace(bufferString, "%LIB_NAME%", _libName);
    replace(bufferString, "%CLASS_NAME%", info.className);
    replace(bufferString, "%PRESET_NAME%", info.shaderName);
    replace(bufferString, "%PRESET_CATEGORY%", info.category);

    ofstream          outfile(info.outputPath);
    std::stringstream iss(bufferString);
    while(iss.good())
    {
        std::string line;
        getline(iss, line, '\n');
        if(line == "\"\"")
            continue;
        if(line.starts_with("%SHADERS%"))
        {
            replace(line, "%SHADERS%", "         ");

            for(const auto& s : shaders)
            {
                string shaderLine(line);
                replace(shaderLine, "%SHADER_NAME%", s.info.className);

                // append preset params
                stringstream paramsLines;
                for(const auto& pp : s.presetParams)
                {
                    string paramLine(".Param(\"%PRESET_KEY%\", \"%PRESET_VALUE%\")");
                    replace(paramLine, "%PRESET_KEY%", pp.first);
                    replace(paramLine, "%PRESET_VALUE%", pp.second);
                    paramsLines << endl << paramLine;
                }
                replace(shaderLine, "%PRESET_PARAMS%", paramsLines.str());
                outfile << shaderLine << endl;
            }
        }
        else if(line.starts_with("%TEXTURES%"))
        {
            replace(line, "%TEXTURES%", "          ");

            for(const auto& t : textures)
            {
                string textureLine(line);
                replace(textureLine, "%TEXTURE_NAME%", t.info.className);

                // append preset params
                stringstream paramsLines;
                for(const auto& pp : t.presetParams)
                {
                    string paramLine(".Param(\"%PRESET_KEY%\", \"%PRESET_VALUE%\")");
                    replace(paramLine, "%PRESET_KEY%", pp.first);
                    replace(paramLine, "%PRESET_VALUE%", pp.second);
                    paramsLines << endl << paramLine;
                }
                replace(textureLine, "%TEXTURE_PARAMS%", paramsLines.str());
                outfile << textureLine << endl;
            }
        }
        else if(line.starts_with("%HEADER"))
        {
            outfile << "ShaderGlass preset " << info.category << " / " << info.shaderName << " imported from " << _libName << ":" << endl;
            outfile << "https://github.com/libretro/slang-shaders/blob/master/" << input.generic_string() << endl;
            outfile << "See original file for credits and usage license. " << endl;
            outfile << "This file is auto-generated, do not modify directly." << endl;
        }
        else
            outfile << line << endl;
    }
    outfile.close();
    cout << "Generated PresetDef " << info.outputPath << endl;
}

vector<string> loadSource(const filesystem::path& input, bool followIncludes)
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
            const auto& includeLines = loadSource(includePath.lexically_normal(), true);
            lines.insert(lines.end(), includeLines.begin(), includeLines.end());
        }
        else
            lines.push_back(line);
    }
    infile.close();

    return lines;
}

void processShader(ShaderDef def)
{
    vector<string> vertexSource;
    vector<string> fragmentSource;

    bool        isVertex = true, isFragment = true;
    const auto& source = loadSource(def.input, true);
    bool inComment = false;
    for(const auto& line : source)
    {
        auto trimLine = trim(line);
        if(line.starts_with("#pragma parameter"))
        {
            def.params.push_back(ShaderParam(line, 1));
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
        else if(trimLine.starts_with("//"))
        {
            if (trimLine.ends_with("*/"))
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
            if (trimLine.ends_with("*/"))
            {
                def.comments.push_back(trimLine.substr(2, trimLine.length() - 4));
            }
            else
            {
                def.comments.push_back(trimLine.substr(2));
                inComment = true;
            }
        }
        else if (trimLine.ends_with("*/"))
        {
            def.comments.push_back(trimLine.substr(0, trimLine.length() - 2));
            inComment = false;
        }
        else if(trimLine.starts_with("*/"))
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
                fragmentSource.push_back(line);
            if(isVertex)
                vertexSource.push_back(line);
        }
    }

    const auto& vertexOutput   = spirv(glsl(def.input, "vert", vertexSource));
    const auto& fragmentOutput = spirv(glsl(def.input, "frag", fragmentSource));
    def.vertexSource           = vertexOutput.first;
    def.vertexMetadata         = vertexOutput.second;
    def.fragmentSource         = fragmentOutput.first;
    def.fragmentMetadata       = fragmentOutput.second;

    filesystem::path metaOutput(def.input);
    metaOutput.replace_extension(".meta");
    saveSource(metaOutput, fragmentOutput.second);

    def.vertexByteCode   = fxc(def.input, "vs_5_0", vertexOutput.first);
    def.fragmentByteCode = fxc(def.input, "ps_5_0", fragmentOutput.first);

    replace(def.vertexByteCode, " ", "");
    replace(def.fragmentByteCode, " ", "");

    populateShaderTemplate(def);
}

string bin2string(filesystem::path input)
{
    ostringstream oss;
    ifstream      infile(input, fstream::binary);
    bool          first   = true;
    int           counter = 0;

    oss << "{";
    char c;
    while(infile.get(c))
    {
        if(first)
        {
            first = false;
        }
        else
        {
            oss << ",";
        }
        oss << (int)((unsigned char)c);
        counter++;
        if(counter == 40)
        {
            counter = 0;
            oss << endl;
        }
        if(infile.eof())
            break;
    }
    oss << "};";
    return oss.str();
}

void processTexture(TextureDef def)
{
    def.data = bin2string(def.input);
    populateTextureTemplate(def);
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

string getValue(const string& key, int shaderNo, const map<string, string>& keyValues)
{
    stringstream ss;
    ss << key;
    if(shaderNo >= 0)
    {
        ss << shaderNo;
    }
    if(keyValues.find(ss.str()) != keyValues.end())
    {
        return keyValues.at(ss.str());
    }
    return string();
}

string getValue(const string& key, const string& suffix, const map<string, string>& keyValues)
{
    stringstream ss;
    ss << key << suffix;
    if(keyValues.find(ss.str()) != keyValues.end())
    {
        return keyValues.at(ss.str());
    }
    return string();
}

void setPresetParam(const string& paramName, ShaderDef& def, int i, const map<string, string>& keyValues)
{
    const auto& value = getValue(paramName, i, keyValues);
    if(!value.empty())
        def.presetParams.insert(make_pair(paramName, value));
}

void setPresetParam(const string& paramName, TextureDef& def, const string& suffix, const map<string, string>& keyValues)
{
    const auto& value = getValue(paramName, suffix, keyValues);
    if(!value.empty())
        def.presetParams.insert(make_pair(suffix, value));
}

void setPresetParams(ShaderDef& def, int i, const map<string, string>& keyValues)
{
    setPresetParam("filter_linear", def, i, keyValues);
    setPresetParam("float_framebuffer", def, i, keyValues);
    setPresetParam("srgb_framebuffer", def, i, keyValues);
    setPresetParam("scale_type", def, i, keyValues);
    setPresetParam("scale", def, i, keyValues);
    setPresetParam("scale_type_x", def, i, keyValues);
    setPresetParam("scale_x", def, i, keyValues);
    setPresetParam("scale_type_y", def, i, keyValues);
    setPresetParam("scale_y", def, i, keyValues);
    setPresetParam("alias", def, i, keyValues);
    setPresetParam("mipmap_input", def, i, keyValues);
    setPresetParam("frame_count_mod", def, i, keyValues);
}

void setPresetParams(TextureDef& def, std::string name, const map<string, string>& keyValues)
{
    setPresetParam(name + "_", def, "linear", keyValues);
    setPresetParam(name + "_", def, "wrap_mode", keyValues);
    setPresetParam(name + "_", def, "mipmap", keyValues);
}

void processPreset(const filesystem::path& input)
{
    map<string, string> keyValues;

    fstream infile(input);
    string  line;
    while(getline(infile, line))
    {
        if(line.starts_with("#"))
        {
            continue;
        }
        keyValues.insert(getKeyValue(line));
    }
    infile.close();

    auto              numShaders = atoi(getValue("shaders", -1, keyValues).c_str());
    vector<ShaderDef> shaders;
    for(int i = 0; i < numShaders; i++)
    {
        const auto& shaderRelativePath = filesystem::path(getValue("shader", i, keyValues));
        auto        shaderFullPath     = input.parent_path();
        shaderFullPath /= shaderRelativePath;
        shaderFullPath.make_preferred();
        shaderFullPath = shaderFullPath.lexically_normal();
        auto def       = ShaderDef(shaderFullPath);
        setPresetParams(def, i, keyValues);
        if(_force || !filesystem::exists(def.info.outputPath))
        {
            processShader(def);
        }
        updateShaderList(def.info);
        shaders.push_back(def);
    }
    auto               pDef = getShaderInfo(input, "PresetDef");
    vector<TextureDef> textures;
    auto               textureList = getValue("textures", -1, keyValues);
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
                const auto& textureRelativePath = filesystem::path(getValue(textureName, "", keyValues));
                auto        textureFullPath     = input.parent_path();
                textureFullPath /= textureRelativePath;
                textureFullPath.make_preferred();
                textureFullPath = textureFullPath.lexically_normal();

                auto def = TextureDef(textureFullPath);
                def.presetParams.insert(make_pair("name", textureName));
                setPresetParams(def, textureName, keyValues);
                if(_force || !filesystem::exists(def.info.outputPath))
                {
                    processTexture(def);
                }
                updateTextureList(def.info);
                textures.push_back(def);

                textureList.erase(0, pos + 1);
            }
        }
    }
    if(_force || !filesystem::exists(pDef.outputPath))
    {
        populatePresetTemplate(input, shaders, textures);
    }
    updatePresetList(getShaderInfo(input, "PresetDef"));
}

void processFile(const filesystem::path& input)
{
    if(input.filename().string()[0] == '-') // exclusions (files)
        return;

    if(input.string()[0] == '-') // exclusions (folders)
        return;

    std::cout << "======= " << input << '\n';
    if(input.extension() == ".slang")
        processShader(input);
    else if(input.extension() == ".slangp")
        processPreset(input);
}

void processListTemplate()
{
    listPath /= filesystem::path(string(_libName) + ".h");
    if(!filesystem::exists(listPath))
    {
        fstream           infile(startupPath / filesystem::path("List.template"));
        std::stringstream buffer;
        buffer << infile.rdbuf();
        auto bufferString = buffer.str();
        replace(bufferString, "%LIB_NAME%", _libName);

        ofstream outfile(listPath);
        outfile << bufferString;
        outfile.close();
        cout << "Generated list " << listPath.string() << endl;
    }
    shaderList = loadSource(listPath, false);
}

int main(int argc, char* argv[])
{
    startupPath = filesystem::current_path();
    filesystem::current_path(_inputPath);

    processListTemplate();

    for(int i = 1; i < argc; i++)
    {
        string input(argv[i]);
        if(input == "force")
        {
            _force = true;
            continue;
        }
        if(input == ".")
        {
            for(auto& p : filesystem::recursive_directory_iterator("."))
            {
                try
                {
                    if(p.path().extension() == ".slangp")
                        processFile(p.path().lexically_normal());
                }
                catch (std::exception& e)
                {
                    cout << e.what() << endl;
                }
            }
        }
        else
        {
            if(filesystem::status(input).type() == filesystem::file_type::directory)
            {
                for(auto& p : filesystem::directory_iterator(input))
                {
                    processFile(p.path());
                }
            }
            else
                processFile(input);
        }
    }
}
