/*
ShaderGlass preset misc / deband imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/misc/deband.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class MiscDebandPresetDef : public PresetDef
{
public:
	MiscDebandPresetDef() : PresetDef{}
	{
		Name = "deband";
		Category = "misc";
	}

	virtual void Build() {
         	ShaderDefs.push_back(MiscShadersDebandShaderDef()
.Param("scale_type", "source"));
	}
};
}
