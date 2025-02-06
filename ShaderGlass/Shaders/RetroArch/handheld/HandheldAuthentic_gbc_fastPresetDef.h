/*
ShaderGlass preset handheld / authentic_gbc_fast imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/handheld/authentic_gbc_fast.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class HandheldAuthentic_gbc_fastPresetDef : public PresetDef
{
public:
	HandheldAuthentic_gbc_fastPresetDef() : PresetDef{}
	{
		Name = "authentic_gbc_fast";
		Category = "handheld";
	}

	virtual void Build() {
         	ShaderDefs.push_back(HandheldShadersAuthentic_gbcTo_lin_fastShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "source")
.Param("srgb_framebuffer", "true"));
         	ShaderDefs.push_back(HandheldShadersAuthentic_gbcAuthentic_gbc_fastShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "viewport"));
	}
};
}
