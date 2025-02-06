/*
ShaderGlass preset handheld-color-mod / NSO-gba-color imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/handheld/color-mod/NSO-gba-color.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class HandheldColorModNSOGbaColorPresetDef : public PresetDef
{
public:
	HandheldColorModNSOGbaColorPresetDef() : PresetDef{}
	{
		Name = "NSO-gba-color";
		Category = "handheld-color-mod";
	}

	virtual void Build() {
         	ShaderDefs.push_back(HandheldShadersColorNSOGbaColorShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
	}
};
}
