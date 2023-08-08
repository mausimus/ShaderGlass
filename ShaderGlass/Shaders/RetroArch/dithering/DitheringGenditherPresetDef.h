/*
ShaderGlass preset dithering / gendither imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/6f921ee4815a7894a33855974285b04545a4fa42/dithering/gendither.slangp
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
