/*
ShaderGlass preset misc / ascii imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/misc/ascii.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class MiscAsciiPresetDef : public PresetDef
{
public:
	MiscAsciiPresetDef() : PresetDef{}
	{
		Name = "ascii";
		Category = "misc";
	}

	virtual void Build() {
         	ShaderDefs.push_back(MiscShadersAsciiShaderDef()
.Param("scale_type", "viewport"));
	}
};
}
