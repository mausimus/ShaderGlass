/*
ShaderGlass preset handheld / authentic_gbc_single_pass imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/handheld/authentic_gbc_single_pass.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class HandheldAuthentic_gbc_single_passPresetDef : public PresetDef
{
public:
	HandheldAuthentic_gbc_single_passPresetDef() : PresetDef{}
	{
		Name = "authentic_gbc_single_pass";
		Category = "handheld";
	}

	virtual void Build() {
         	ShaderDefs.push_back(HandheldShadersAuthentic_gbcAuthentic_gbc_single_passShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "viewport"));
	}
};
}
