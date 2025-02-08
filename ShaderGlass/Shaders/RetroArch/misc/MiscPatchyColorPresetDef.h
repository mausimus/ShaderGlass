/*
ShaderGlass preset misc / patchy-color imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/misc/patchy-color.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class MiscPatchyColorPresetDef : public PresetDef
{
public:
	MiscPatchyColorPresetDef() : PresetDef{}
	{
		Name = "patchy-color";
		Category = "misc";
	}

	virtual void Build() {
         	ShaderDefs.push_back(NtscShadersPatchyNtscPatchyColorShaderDef()
.Param("filter_linear", "false")
.Param("float_framebuffer", "true")
.Param("mipmap_input", "false")
.Param("scale_type_x", "source")
.Param("scale_type_y", "source")
.Param("scale_x", "1.000000")
.Param("scale_y", "1.000000")
.Param("srgb_framebuffer", "false")
.Param("wrap_mode", "clamp_to_border"));
         	ShaderDefs.push_back(NtscShadersPatchyNtscTrilinearLUTSwitchableShaderDef()
.Param("filter_linear", "false")
.Param("float_framebuffer", "true")
.Param("mipmap_input", "false")
.Param("scale_type_x", "source")
.Param("scale_type_y", "source")
.Param("scale_x", "1.000000")
.Param("scale_y", "1.000000")
.Param("srgb_framebuffer", "false")
.Param("wrap_mode", "clamp_to_border"));
         	ShaderDefs.push_back(NtscShadersPatchyNtscLinearToSrgbShaderDef()
.Param("filter_linear", "false")
.Param("float_framebuffer", "true")
.Param("mipmap_input", "false")
.Param("scale_type_x", "source")
.Param("scale_type_y", "source")
.Param("scale_x", "1.000000")
.Param("scale_y", "1.000000")
.Param("srgb_framebuffer", "false")
.Param("wrap_mode", "clamp_to_border"));
            TextureDefs.push_back(NtscShadersPatchyNtscP22_80s_D65TextureDef()
.Param("linear", "false")
.Param("mipmap", "false")
.Param("name", "PhosphorSamplerLUT1")
.Param("wrap_mode", "clamp_to_border"));
            TextureDefs.push_back(NtscShadersPatchyNtscP22_90s_D65TextureDef()
.Param("linear", "false")
.Param("mipmap", "false")
.Param("name", "PhosphorSamplerLUT2")
.Param("wrap_mode", "clamp_to_border"));
            TextureDefs.push_back(NtscShadersPatchyNtscP22_J_D65TextureDef()
.Param("linear", "false")
.Param("mipmap", "false")
.Param("name", "PhosphorSamplerLUT3")
.Param("wrap_mode", "clamp_to_border"));
            TextureDefs.push_back(NtscShadersPatchyNtscTrinitronP22_D65TextureDef()
.Param("linear", "false")
.Param("mipmap", "false")
.Param("name", "PhosphorSamplerLUT4")
.Param("wrap_mode", "clamp_to_border"));
            TextureDefs.push_back(NtscShadersPatchyNtscP22_J_D93TextureDef()
.Param("linear", "false")
.Param("mipmap", "false")
.Param("name", "PhosphorSamplerLUT5")
.Param("wrap_mode", "clamp_to_border"));
            TextureDefs.push_back(NtscShadersPatchyNtscTrinitronP22_D93TextureDef()
.Param("linear", "false")
.Param("mipmap", "false")
.Param("name", "PhosphorSamplerLUT6")
.Param("wrap_mode", "clamp_to_border"));
            OverrideParam("feedback_pass", (float)0.000000);
	}
};
}
