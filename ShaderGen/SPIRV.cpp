#include "SPIRV.h"

#ifdef USE_SPIRV_LIB

#pragma comment(lib, "spirv-cross-core.lib")
#pragma comment(lib, "spirv-cross-hlsl.lib")
#pragma comment(lib, "spirv-cross-glsl.lib")
#pragma comment(lib, "spirv-cross-reflect.lib")

#include "include/spirv_hlsl.hpp"
#include "include/spirv_reflect.hpp"

using namespace SPIRV_CROSS_NAMESPACE;

std::pair<std::string, std::string> SPIRV::GenerateHLSL(const std::vector<uint32_t>& bin, bool fragment)
{
    CompilerHLSL hlsl(bin);

    CompilerHLSL::Options options;
    options.shader_model = 50;
    hlsl.set_hlsl_options(options);
    std::string source = hlsl.compile();

    CompilerReflection refl(bin);

    std::string metadata = refl.compile();
    return std::make_pair(source, metadata);
}

#endif
