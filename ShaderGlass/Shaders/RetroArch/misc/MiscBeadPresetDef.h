/*
ShaderGlass preset misc / bead imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/misc/bead.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class MiscBeadPresetDef : public PresetDef
{
public:
	MiscBeadPresetDef() : PresetDef{}
	{
		Name = "bead";
		Category = "misc";
	}

	virtual void Build() {
         	ShaderDefs.push_back(MiscShadersBeadShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "viewport"));
	}
};
}
