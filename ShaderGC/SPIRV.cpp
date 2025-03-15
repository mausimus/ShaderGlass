#include "pch.h"

#include "SPIRV.h"

#include "include/spirv_hlsl.hpp"
#include "include/spirv_reflect.hpp"

#ifdef _DEBUG
#    pragma comment(lib, "spirv-cross-cored.lib")
#    pragma comment(lib, "spirv-cross-hlsld.lib")
#    pragma comment(lib, "spirv-cross-glsld.lib")
#    pragma comment(lib, "spirv-cross-reflectd.lib")
#else
#    pragma comment(lib, "spirv-cross-core.lib")
#    pragma comment(lib, "spirv-cross-hlsl.lib")
#    pragma comment(lib, "spirv-cross-glsl.lib")
#    pragma comment(lib, "spirv-cross-reflect.lib")
#endif

using namespace SPIRV_CROSS_NAMESPACE;

std::pair<std::string, std::string> SPIRV::GenerateHLSL(const std::vector<uint32_t>& bin, bool fragment, std::ostream& log, bool& warn)
{
    //std::cout << "GenerateHLSL...";

    try
    {
        CompilerHLSL hlsl(bin);

        CompilerHLSL::Options options;
        options.shader_model = 50;        
        hlsl.set_hlsl_options(options);
        std::string source = hlsl.compile();

        std::string metadata;
        if(fragment)
        {
            CompilerReflection refl(bin);
            metadata = refl.compile();
        }

        //std::cout << "OK" << std::endl;

        return std::make_pair(source, metadata);
    }
    catch(std::exception& ex)
    {
        std::ostringstream msg;
        msg << "HLSL Generation Error" << std::endl;
        msg << ex.what();
        throw std::runtime_error(msg.str());
    }
}
