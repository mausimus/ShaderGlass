/*
ShaderGlass preset dithering / gendither imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/dithering/gendither.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class DitheringGenditherPresetDef : public PresetDef
{
public:
	DitheringGenditherPresetDef() : PresetDef{}
	{
		Name = "gendither";
		Category = "dithering";
	}

	virtual void Build() {
         	ShaderDefs.push_back(DitheringShadersGenditherShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "source"));
	}
};
}
