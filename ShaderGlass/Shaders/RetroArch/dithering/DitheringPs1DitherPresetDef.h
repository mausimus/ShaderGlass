/*
ShaderGlass preset dithering / ps1-dither imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/dithering/ps1-dither.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class DitheringPs1DitherPresetDef : public PresetDef
{
public:
	DitheringPs1DitherPresetDef() : PresetDef{}
	{
		Name = "ps1-dither";
		Category = "dithering";
	}

	virtual void Build() {
         	ShaderDefs.push_back(DitheringShadersPs1ditherPS1UnditherAntiBayerShaderDef()
.Param("filter_linear", "true")
.Param("scale", "1.0")
.Param("scale_type", "source"));
	}
};
}
