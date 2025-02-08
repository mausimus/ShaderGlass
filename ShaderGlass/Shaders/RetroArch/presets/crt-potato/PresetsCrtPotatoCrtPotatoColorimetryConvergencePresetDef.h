/*
ShaderGlass preset presets-crt-potato / crt-potato-colorimetry-convergence imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/presets/crt-potato/crt-potato-colorimetry-convergence.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class PresetsCrtPotatoCrtPotatoColorimetryConvergencePresetDef : public PresetDef
{
public:
	PresetsCrtPotatoCrtPotatoColorimetryConvergencePresetDef() : PresetDef{}
	{
		Name = "crt-potato-colorimetry-convergence";
		Category = "presets-crt-potato";
	}

	virtual void Build() {
         	ShaderDefs.push_back(CrtShadersCrtPotatoShaderFilesCrtPotatoShaderDef()
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
         	ShaderDefs.push_back(MiscShadersColorimetryShaderDef()
.Param("float_framebuffer", "false")
.Param("mipmap_input", "false")
.Param("scale_type_x", "source")
.Param("scale_type_y", "source")
.Param("scale_x", "1.000000")
.Param("scale_y", "1.000000")
.Param("srgb_framebuffer", "false")
.Param("wrap_mode", "clamp_to_border"));
         	ShaderDefs.push_back(MiscShadersConvergenceShaderDef()
.Param("filter_linear", "true")
.Param("float_framebuffer", "false")
.Param("mipmap_input", "false")
.Param("scale_type_x", "source")
.Param("scale_type_y", "source")
.Param("scale_x", "1.000000")
.Param("scale_y", "1.000000")
.Param("srgb_framebuffer", "false")
.Param("wrap_mode", "clamp_to_border"));
            TextureDefs.push_back(CrtShadersCrtPotatoResourcesCrtPotatoThickTextureDef()
.Param("mipmap", "false")
.Param("name", "MASK")
.Param("wrap_mode", "clamp_to_border"));
            OverrideParam("feedback_pass", (float)0.000000);
	}
};
}
