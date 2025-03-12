#include "GLSL.h"

#ifdef USE_GLSL_LIB

#pragma comment(lib, "glslang.lib")
#pragma comment(lib, "glslang-default-resource-limits.lib")

#include "include/glslang/Include/glslang_c_interface.h"
#include "include/glslang/Public/resource_limits_c.h"

#include <cstdio>
#include <cstdint>

SpirVShader GLSL::GenerateSPIRV(const char* vertexSource, const char* fragmentSource)
{
    SpirVShader shader;
    shader.vertexData = GenerateSPIRV(vertexSource, false);
    shader.fragmentData = GenerateSPIRV(vertexSource, false);
    return shader;
}

std::vector<uint32_t> GLSL::GenerateSPIRV(const char* source, bool fragment)
{
    std::vector<uint32_t> bin;
    auto                  stage = fragment ? GLSLANG_STAGE_FRAGMENT : GLSLANG_STAGE_VERTEX;

    const char*           fileName = "dummy";
    const glslang_input_t input    = {
           .language                          = GLSLANG_SOURCE_GLSL,
           .stage                             = stage,
           .client                            = GLSLANG_CLIENT_VULKAN,
           .client_version                    = GLSLANG_TARGET_VULKAN_1_2,
           .target_language                   = GLSLANG_TARGET_SPV,
           .target_language_version           = GLSLANG_TARGET_SPV_1_5,
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
        printf("GLSL preprocessing failed %s\n", fileName);
        printf("%s\n", glslang_shader_get_info_log(shader));
        printf("%s\n", glslang_shader_get_info_debug_log(shader));
        printf("%s\n", input.code);
        glslang_shader_delete(shader);
        return bin;
    }

    if(!glslang_shader_parse(shader, &input))
    {
        printf("GLSL parsing failed %s\n", fileName);
        printf("%s\n", glslang_shader_get_info_log(shader));
        printf("%s\n", glslang_shader_get_info_debug_log(shader));
        printf("%s\n", glslang_shader_get_preprocessed_code(shader));
        glslang_shader_delete(shader);
        return bin;
    }

    glslang_program_t* program = glslang_program_create();
    glslang_program_add_shader(program, shader);

    if(!glslang_program_link(program, GLSLANG_MSG_SPV_RULES_BIT | GLSLANG_MSG_VULKAN_RULES_BIT))
    {
        printf("GLSL linking failed %s\n", fileName);
        printf("%s\n", glslang_program_get_info_log(program));
        printf("%s\n", glslang_program_get_info_debug_log(program));
        glslang_program_delete(program);
        glslang_shader_delete(shader);
        return bin;
    }

    glslang_program_SPIRV_generate(program, stage);

    auto size = glslang_program_SPIRV_get_size(program);
    bin.resize(size);
    glslang_program_SPIRV_get(program, bin.data());

    const char* spirv_messages = glslang_program_SPIRV_get_messages(program);
    if(spirv_messages)
        printf("(%s) %s\b", fileName, spirv_messages);

    glslang_program_delete(program);
    glslang_shader_delete(shader);

    return bin;
}

#endif
