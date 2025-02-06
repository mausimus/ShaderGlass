/*
ShaderGlass preset handheld-color-mod / NSO-gbc-color imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/handheld/color-mod/NSO-gbc-color.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class HandheldColorModNSOGbcColorPresetDef : public PresetDef
{
public:
	HandheldColorModNSOGbcColorPresetDef() : PresetDef{}
	{
		Name = "NSO-gbc-color";
		Category = "handheld-color-mod";
	}

	virtual void Build() {
         	ShaderDefs.push_back(HandheldShadersColorNSOGbcColorShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
	}
};
}
