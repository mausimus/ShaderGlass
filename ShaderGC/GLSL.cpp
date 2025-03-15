#include "pch.h"

#include "GLSL.h"

#include "include/glslang/Include/glslang_c_interface.h"
#include "include/glslang/Public/resource_limits_c.h"

#ifdef _DEBUG
#pragma comment(lib, "glslangd.lib")
#pragma comment(lib, "glslang-default-resource-limitsd.lib")
#else
#pragma comment(lib, "glslang.lib")
#pragma comment(lib, "glslang-default-resource-limits.lib")
#endif

#include <cstdio>
#include <cstdint>

std::vector<uint32_t> GLSL::GenerateSPIRV(const char* source, bool fragment, std::ostream& log, bool& warn)
{
    //std::cout << "GenerateSPIRV...";

    std::vector<uint32_t> bin;
    auto                  stage = fragment ? GLSLANG_STAGE_FRAGMENT : GLSLANG_STAGE_VERTEX;

    const glslang_input_t input    = {
           .language                          = GLSLANG_SOURCE_GLSL,
           .stage                             = stage,
           .client                            = GLSLANG_CLIENT_VULKAN,
           .client_version                    = GLSLANG_TARGET_VULKAN_1_0,
           .target_language                   = GLSLANG_TARGET_SPV,
           .target_language_version           = GLSLANG_TARGET_SPV_1_0,
           .code                              = source,
           .default_version                   = 100,
           .default_profile                   = GLSLANG_NO_PROFILE,
           .force_default_version_and_profile = false,
           .forward_compatible                = false,
           .messages                          = GLSLANG_MSG_DEFAULT_BIT,
           .resource                          = glslang_default_resource(),
    };

    glslang_shader_t* shader = glslang_shader_create(&input);

    if(!glslang_shader_preprocess(shader, &input))
    {
        std::ostringstream msg;
        msg << "GLSL preprocessing failed:" << std::endl;
        msg << glslang_shader_get_info_log(shader) << std::endl;
        msg << glslang_shader_get_info_debug_log(shader) << std::endl;
        glslang_shader_delete(shader);
        throw std::runtime_error(msg.str());
    }

    if(!glslang_shader_parse(shader, &input))
    {
        std::ostringstream msg;
        msg << "GLSL parsing failed:" << std::endl;
        msg << glslang_shader_get_info_log(shader) << std::endl;
        msg << glslang_shader_get_info_debug_log(shader) << std::endl;
        glslang_shader_delete(shader);
        throw std::runtime_error(msg.str());
    }

    glslang_program_t* program = glslang_program_create();
    glslang_program_add_shader(program, shader);

    if(!glslang_program_link(program, GLSLANG_MSG_SPV_RULES_BIT | GLSLANG_MSG_VULKAN_RULES_BIT))
    {
        std::ostringstream msg;
        msg << "GLSL linking  failed:" << std::endl;
        msg << glslang_program_get_info_log(program) << std::endl;
        msg << glslang_program_get_info_debug_log(program) << std::endl;
        glslang_program_delete(program);
        glslang_shader_delete(shader);
        throw std::runtime_error(msg.str());
    }

    glslang_program_SPIRV_generate(program, stage);

    auto size = glslang_program_SPIRV_get_size(program);
    bin.resize(size);
    glslang_program_SPIRV_get(program, bin.data());

    const char* spirv_messages = glslang_program_SPIRV_get_messages(program);
    if(spirv_messages)
        log << spirv_messages << std::endl;

    glslang_program_delete(program);
    glslang_shader_delete(shader);

    //std::cout << "OK" << std::endl;

    return bin;
}
