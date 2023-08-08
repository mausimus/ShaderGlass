/*
ShaderGlass preset dithering / jinc2-dedither imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/6f921ee4815a7894a33855974285b04545a4fa42/dithering/jinc2-dedither.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class DitheringJinc2DeditherPresetDef : public PresetDef
{
public:
	DitheringJinc2DeditherPresetDef() : PresetDef{}
	{
		Name = "jinc2-dedither";
		Category = "dithering";
	}

	virtual void Build() {
         	ShaderDefs.push_back(DitheringShadersJinc2DeditherShaderDef()
.Param("scale", "1.0")
.Param("scale_type", "source"));
	}
};
}
