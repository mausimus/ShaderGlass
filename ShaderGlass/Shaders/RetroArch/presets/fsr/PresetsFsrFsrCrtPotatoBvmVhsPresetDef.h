/*
ShaderGlass preset presets-fsr / fsr-crt-potato-bvm-vhs imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/presets/fsr/fsr-crt-potato-bvm-vhs.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class PresetsFsrFsrCrtPotatoBvmVhsPresetDef : public PresetDef
{
public:
	PresetsFsrFsrCrtPotatoBvmVhsPresetDef() : PresetDef{}
	{
		Name = "fsr-crt-potato-bvm-vhs";
		Category = "presets-fsr";
	}

	virtual void Build() {
         	ShaderDefs.push_back(EdgeSmoothingFsrShadersFsrPass0ShaderDef()
.Param("filter_linear", "true")
.Param("float_framebuffer", "false")
.Param("mipmap_input", "false")
.Param("scale_type_x", "viewport")
.Param("scale_type_y", "viewport")
.Param("scale_x", "1.000000")
.Param("scale_y", "1.000000")
.Param("srgb_framebuffer", "false")
.Param("wrap_mode", "clamp_to_edge"));
         	ShaderDefs.push_back(EdgeSmoothingFsrShadersFsrPass1ShaderDef()
.Param("filter_linear", "true")
.Param("float_framebuffer", "false")
.Param("mipmap_input", "false")
.Param("scale_type_x", "source")
.Param("scale_type_y", "source")
.Param("scale_x", "1.000000")
.Param("scale_y", "1.000000")
.Param("srgb_framebuffer", "false")
.Param("wrap_mode", "clamp_to_edge"));
         	ShaderDefs.push_back(CrtShadersCrtPotatoShaderFilesUltra_potatoShaderDef()
.Param("alias", "PASS0")
.Param("filter_linear", "false")
.Param("float_framebuffer", "false")
.Param("mipmap_input", "false")
.Param("scale_type_x", "viewport")
.Param("scale_type_y", "viewport")
.Param("scale_x", "1.000000")
.Param("scale_y", "1.000000")
.Param("srgb_framebuffer", "false")
.Param("wrap_mode", "clamp_to_border"));
         	ShaderDefs.push_back(VhsShadersVhsShaderDef()
.Param("filter_linear", "true")
.Param("float_framebuffer", "false")
.Param("mipmap_input", "false")
.Param("srgb_framebuffer", "false")
.Param("wrap_mode", "clamp_to_border"));
         	ShaderDefs.push_back(VhsShadersRewindShaderDef()
.Param("filter_linear", "true")
.Param("float_framebuffer", "false")
.Param("mipmap_input", "false")
.Param("scale_type_x", "source")
.Param("scale_type_y", "source")
.Param("scale_x", "1.000000")
.Param("scale_y", "1.000000")
.Param("srgb_framebuffer", "false")
.Param("wrap_mode", "clamp_to_border"));
            TextureDefs.push_back(CrtShadersCrtPotatoResourcesMask1TextureDef()
.Param("mipmap", "false")
.Param("name", "MASK")
.Param("wrap_mode", "clamp_to_border"));
            TextureDefs.push_back(VhsResourcesRewTextureDef()
.Param("mipmap", "false")
.Param("name", "rew")
.Param("wrap_mode", "clamp_to_border"));
            TextureDefs.push_back(VhsResourcesPlayTextureDef()
.Param("mipmap", "false")
.Param("name", "play")
.Param("wrap_mode", "clamp_to_border"));
            OverrideParam("feedback_pass", (float)0.000000);
            OverrideParam("smear", (float)0.300000);
	}
};
}
