/*
ShaderGen: slangp shader converter for ShaderGlass
https://github.com/mausimus/ShaderGlass
GNU General Public License v3.0
*/

#include "ShaderGen.h"

#include "ShaderGC.h"
#include "GLSL.h"
#include "SPIRV.h"
#include "HLSL.h"

filesystem::path startupPath;
filesystem::path templatePath;
filesystem::path toolsPath;
filesystem::path tempPath;
filesystem::path reportPath;
filesystem::path listPath;
vector<string>   shaderList;

std::string exec(const char* cmd, ofstream& log)
{
    std::array<char, 128> buffer;
    std::string           result;
    log << cmd << endl;
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

filesystem::path glsl(const filesystem::path& shaderPath, const string& stage, const string& source, ofstream& log, bool& warn)
{
    filesystem::path input = tempPath / shaderPath;
    input.replace_extension("." + stage + ".glsl");
    filesystem::path output = tempPath / shaderPath;
    output.replace_extension("." + stage + ".spv");
    filesystem::create_directories(output.parent_path());

    saveSource(input, source);

    if(_tools)
    {
        stringstream cmd;
        cmd << "\"" << toolsPath.string() << _glslExe << "\" "
            << "-V --quiet -S " << stage << " -o " << output.string() << " " << input.string() << "";
        auto        cmds      = cmd.str();
        auto        cmdstring = cmds.c_str();
        const auto& result    = exec(cmdstring, log);
        if(result.length() > 0)
            log << result << endl;
        if(result.contains("error") || result.contains("ERROR"))
            throw std::runtime_error("SPIR-V conversion error");
    }
    else
    {
        auto bin = GLSL::GenerateSPIRV(source.c_str(), stage == "frag", log, warn);
        if(bin.empty())
            throw std::runtime_error("SPIR-V conversion error");

        ofstream out(output, ios::binary | ios::trunc);
        out.write((const char*)bin.data(), bin.size() * sizeof(uint32_t));
        out.close();
    }
    return output;
}

pair<string, string> spirv(const filesystem::path& input, const std::string& stage, ofstream& log, bool& warn)
{
    if(_tools)
    {
        stringstream cmd1, cmd2;
        cmd1 << "\"" << toolsPath.string() << _spirvExe << "\" "
             << " --hlsl --shader-model 50 " << input.string() << "";
        const auto& code = exec(cmd1.str().c_str(), log);
        std::string metadata;
        if(stage == "frag")
        {
            cmd2 << "\"" << toolsPath.string() << _spirvExe << "\" " << input.string() << " --reflect";
            metadata = exec(cmd2.str().c_str(), log);
        }
        return make_pair(code, metadata);
    }
    else
    {
        ifstream inf(input, ios::binary | ios::ate);
        int      size = inf.tellg();
        inf.seekg(0, ios::beg);
        vector<uint32_t> buffer;
        buffer.resize(size / sizeof(uint32_t));
        inf.read((char*)buffer.data(), size);
        inf.close();
        return SPIRV::GenerateHLSL(buffer, stage == "frag", log, warn);
    }
}

string fxc(const filesystem::path& shaderPath, const string& profile, const string& source, ofstream& log, bool& warn)
{
    filesystem::path input = tempPath / shaderPath;
    input.replace_extension("." + profile + ".hlsl");
    filesystem::path output = tempPath / shaderPath;
    output.replace_extension("." + profile + ".h");

    // 3557 - forcing loop to unroll
    // 3570 - gradient instruction used in a loop with varying iteration
    // 3571 - pow with negative
    // 4000 - use of potentially uninitialized variable
    // 4008 - floating point division by zero
    auto fullSource = std::string("#pragma warning (disable : 3557)\r\n#pragma warning (disable : 3570)\r\n#pragma warning (disable : 3571)\r\n#pragma warning (disable : "
                                  "4000)\r\n#pragma warning (disable : 4008)\r\n#pragma warning (disable : 3556)\r\n" +
                                  source);
    saveSource(input, fullSource);

    if(_tools)
    {
        stringstream cmd;
        cmd << "\"" << _fxcPath << "\" "
            << " /nologo /O3 /E main /T " << profile << " /Fh " << output.string() << " " << input.string() << " 2>&1";
        const auto& result = exec(cmd.str().c_str(), log);
        if(result.length() > 0)
            log << result << endl;
        if(result.contains("error") || result.contains("ERROR"))
            throw std::runtime_error("FXC compilation error");
        if(result.contains("warn"))
            warn = true;

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
    else
    {
        auto bin = HLSL::CompileHLSL(fullSource.c_str(), fullSource.size(), profile.c_str(), log, warn);

        ostringstream sbuf;
        sbuf << "{" << endl;
        int nl = 0;
        for(auto b : bin)
        {
            sbuf << (int)b;
            if(nl < bin.size() - 1)
            {
                sbuf << ",";
                if(++nl % 6 == 0)
                    sbuf << endl;
            }
        }
        sbuf << endl << "};" << endl;

        auto str = sbuf.str();

        ofstream outf(output);
        outf << str;
        outf.close();

        return str;
    }
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

void updateShaderList(const SourceShaderInfo& shaderInfo)
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

void updateTextureList(const SourceShaderInfo& textureInfo)
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

void updatePresetList(const SourceShaderInfo& shaderInfo)
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

void populateShaderTemplate(SourceShaderDef def, ofstream& log)
{
    const auto& info = def.info;

    fstream           infile(templatePath / filesystem::path("Shader.template"));
    std::stringstream buffer;
    buffer << infile.rdbuf();
    auto bufferString = buffer.str();
    replace(bufferString, "%LIB_NAME%", _libName);
    replace(bufferString, "%CLASS_NAME%", info.className);
    replace(bufferString, "%SHADER_NAME%", info.shaderName);
    replace(bufferString, "%SHADER_FORMAT%", def.format);
    replace(bufferString, "%SHADER_CATEGORY%", info.category);
    replace(bufferString, "%VERTEX_SOURCE%", splitCode(def.vertexSource));
    replace(bufferString, "%FRAGMENT_SOURCE%", splitCode(def.fragmentSource));
    replace(bufferString, "%VERTEX_BYTECODE%", def.vertexByteCode);
    replace(bufferString, "%FRAGMENT_BYTECODE%", def.fragmentByteCode);

    if(def.fragmentByteCode.empty() || def.vertexByteCode.empty())
    {
        throw std::runtime_error("Shader compilation failed");
    }

    std::vector<SourceShaderSampler> textures;
    def.params = ShaderGC::LookupParams(def.params, textures, def.fragmentMetadata);

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
            outfile << _raUrl << def.input.generic_string() << endl;
            outfile << "See original file for full credits and usage license with excerpts below. " << endl;
            outfile << "This file is auto-generated, do not modify directly." << endl;
            if(def.comments.size())
            {
                outfile << endl;
                for(const auto& c : def.comments)
                    outfile << c << endl;
            }
            outfile << endl;
        }
        else
            outfile << line << endl;
    }
    outfile.close();
    log << "Generated ShaderDef " << info.outputPath << endl;
}

void populateTextureTemplate(SourceTextureDef def, ofstream& log)
{
    const auto& info = def.info;

    fstream           infile(templatePath / filesystem::path("Texture.template"));
    std::stringstream buffer;
    buffer << infile.rdbuf();
    auto bufferString = buffer.str();
    replace(bufferString, "%LIB_NAME%", _libName);
    replace(bufferString, "%TEXTURE_NAME%", def.input.filename().string());
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
            outfile << _raUrl << def.input.generic_string() << endl;
            outfile << "See original file for credits and usage license. " << endl;
            outfile << "This file is auto-generated, do not modify directly." << endl;
        }
        else
            outfile << line << endl;
    }
    outfile.close();
    log << "Generated TextureDef " << info.outputPath << endl;
}

void populatePresetTemplate(
    const filesystem::path& input, const vector<SourceShaderDef>& shaders, const vector<SourceTextureDef>& textures, const vector<SourceShaderParam>& overrides, ofstream& log)
{
    const auto& info = getShaderInfo(input, "PresetDef");

    fstream           infile(templatePath / filesystem::path("Preset.template"));
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
        else if(line.starts_with("%OVERRIDES%"))
        {
            replace(line, "%OVERRIDES%", "           ");

            for(const auto& o : overrides)
            {
                string overrideLine(line);
                replace(overrideLine, "%OVERRIDE_NAME%", o.name);
                replace(overrideLine, "%OVERRIDE_VALUE%", to_string(o.def));
                outfile << overrideLine << endl;
            }
        }
        else if(line.starts_with("%HEADER"))
        {
            outfile << "ShaderGlass preset " << info.category << " / " << info.shaderName << " imported from " << _libName << ":" << endl;
            outfile << _raUrl << input.generic_string() << endl;
            outfile << "See original file for credits and usage license. " << endl;
            outfile << "This file is auto-generated, do not modify directly." << endl;
        }
        else
            outfile << line << endl;
    }
    outfile.close();
    log << "Generated PresetDef " << info.outputPath << endl;
}

void processShader(SourceShaderDef def, ofstream& log, bool& warn)
{
    try
    {
        ShaderGC::ProcessSourceShader(def, log, warn);

        const auto& vertexOutput   = spirv(glsl(def.input, "vert", def.vertexSource, log, warn), "vert", log, warn);
        const auto& fragmentOutput = spirv(glsl(def.input, "frag", def.fragmentSource, log, warn), "frag", log, warn);
        def.vertexSource           = vertexOutput.first;
        def.vertexMetadata         = vertexOutput.second;
        def.fragmentSource         = fragmentOutput.first;
        def.fragmentMetadata       = fragmentOutput.second;

        filesystem::path metaOutput(tempPath / def.input);
        metaOutput.replace_extension(".meta");
        saveSource(metaOutput, fragmentOutput.second);

        def.vertexByteCode   = fxc(def.input, "vs_5_0", vertexOutput.first, log, warn);
        def.fragmentByteCode = fxc(def.input, "ps_5_0", fragmentOutput.first, log, warn);

        replace(def.vertexByteCode, " ", "");
        replace(def.fragmentByteCode, " ", "");

        populateShaderTemplate(def, log);
    }
    catch(std::runtime_error& ex)
    {
        log << def.input << endl;
        throw ex;
    }
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

void processTexture(SourceTextureDef def, ofstream& log)
{
    def.data = bin2string(def.input);
    populateTextureTemplate(def, log);
}

void processPreset(SourcePresetDef& def, ofstream& log, bool& warn)
{
    ShaderGC::ProcessSourcePreset(def, log, warn);

    for(auto& s : def.shaders)
    {
        s.info = getShaderInfo(s.input, "ShaderDef");
        if(_force || !filesystem::exists(s.info.outputPath))
        {
            processShader(s, log, warn);
        }
        updateShaderList(s.info);
    }

    for(auto& t : def.textures)
    {
        t.info = getShaderInfo(t.input, "TextureDef");
        if(_force || !filesystem::exists(t.info.outputPath))
        {
            processTexture(t, log);
        }
        updateTextureList(t.info);
    }

    def.info = getShaderInfo(def.input, "PresetDef");
    if(_force || !filesystem::exists(def.info.outputPath))
    {
        populatePresetTemplate(def.input, def.shaders, def.textures, def.overrides, log);
    }
    updatePresetList(def.info);
}

void processFile(const filesystem::path& input, ofstream& reportStream)
{
    if(input.filename().string()[0] == '-') // exclusions (files)
        return;

    if(input.string()[0] == '-') // exclusions (folders)
        return;

    if(!filesystem::exists(input))
    {
        cout << "Cannot find file " << input << endl;
        return;
    }

    auto inputString = input.string();
    std::replace(inputString.begin(), inputString.end(), '\\', '!');
    std::filesystem::path logPath(tempPath / "logs" / (inputString + ".log"));
    std::filesystem::create_directory(tempPath / "logs");
    ofstream log(logPath);
    bool     warn = false;
    bool     err  = false;

    try
    {
        std::cout << input << " ...";
        if(input.extension() == ".slang")
        {
            SourceShaderDef sd(input, getShaderInfo(input, "ShaderDef"));
            processShader(sd, log, warn);
        }
        else if(input.extension() == ".slangp")
        {
            SourcePresetDef sd(input, getShaderInfo(input, "PresetDef"));
            processPreset(sd, log, warn);
        }

        log << "OK" << endl;
    }
    catch(std::exception& e)
    {
        cout << e.what() << endl;
        err = true;

        log << "ERROR:" << e.what() << endl;
    }
    log.close();

    if(err)
    {
        auto orgPath(logPath);
        std::filesystem::rename(orgPath, logPath.replace_extension(".ERROR.log"));
        std::cout << "ERROR" << endl;
        reportStream << "ERROR: " << input << endl;
    }
    else if(warn)
    {
        auto orgPath(logPath);
        std::filesystem::rename(orgPath, logPath.replace_extension(".WARN.log"));
        std::cout << "WARN" << endl;
        reportStream << "WARN: " << input << endl;
    }
    else
    {
        std::cout << "OK" << endl;
        reportStream << "OK: " << input << endl;
    }
}

void processListTemplate()
{
    listPath /= filesystem::path(string(_libName) + ".h");
    if(!filesystem::exists(listPath))
    {
        fstream           infile(templatePath / filesystem::path("List.template"));
        std::stringstream buffer;
        buffer << infile.rdbuf();
        auto bufferString = buffer.str();
        replace(bufferString, "%LIB_NAME%", _libName);

        ofstream outfile(listPath);
        outfile << bufferString;
        outfile.close();
        std::cout << "Generated list " << listPath.string() << endl;
    }
    shaderList = ShaderGC::LoadSource(listPath, false);
}

int main(int argc, char* argv[])
{
    startupPath = filesystem::current_path();
    filesystem::create_directory(_tempPath);
    templatePath = (startupPath / filesystem::path(_templatePath)).lexically_normal();
    tempPath     = (startupPath / filesystem::path(_tempPath)).lexically_normal();
    toolsPath    = (startupPath / filesystem::path(_toolsPath)).lexically_normal();
    listPath     = (startupPath / filesystem::path(_outputPath)).lexically_normal();
    outputPath   = (startupPath / filesystem::path(_outputPath)).lexically_normal();

    filesystem::current_path(_inputPath);
    reportPath = tempPath / (std::format("{:%Y%m%d_%H%M%S}", std::chrono::system_clock::now()) + ".log");
    ofstream reportStream(reportPath);
    reportStream << "Starting at " << (std::format("{:%Y-%m-%d %H:%M:%S}", std::chrono::system_clock::now())) << endl;

    processListTemplate();

    try
    {
        for(int i = 1; i < argc; i++)
        {
            string input(argv[i]);
            if(input == "-force")
            {
                _force = true;
                continue;
            }
            if(input == "-tools")
            {
                if(!filesystem::exists(_fxcPath))
                {
                    cout << "Cannot find fxc.exe! Make sure you have the necessary Window 10 SDK version installed" << endl;
                    cout << _fxcPath << endl;
                    return -1;
                }

                _tools = true;
                continue;
            }
            if(input == "*")
            {
                for(auto& p : filesystem::recursive_directory_iterator("."))
                {
                    if(p.path().extension() == ".slangp")
                    {
                        auto isExcluded  = false;
                        auto excludePath = p.path().parent_path();
                        do
                        {
                            isExcluded |= filesystem::exists(excludePath / ".exclude");
                            excludePath = excludePath.parent_path();
                        } while(!isExcluded && !excludePath.empty());

                        if(_force || !isExcluded)
                        {
                            processFile(p.path().lexically_normal(), reportStream);
                        }
                    }
                }
            }
            else
            {
                if(filesystem::status(input).type() == filesystem::file_type::directory)
                {
                    for(auto& p : filesystem::directory_iterator(input))
                    {
                        processFile(p.path(), reportStream);
                    }
                }
                else
                    processFile(input, reportStream);
            }
        }
    }
    catch(exception& e)
    {
        reportStream << "EXCEPTION: " << e.what() << endl;
    }

    reportStream << "Finishing at " << (std::format("{:%Y-%m-%d %H:%M:%S}", std::chrono::system_clock::now())) << endl;
    reportStream.close();
}
