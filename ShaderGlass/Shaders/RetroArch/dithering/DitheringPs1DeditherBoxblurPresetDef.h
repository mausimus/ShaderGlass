/*
ShaderGlass preset dithering / ps1-dedither-boxblur imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/dithering/ps1-dedither-boxblur.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class DitheringPs1DeditherBoxblurPresetDef : public PresetDef
{
public:
	DitheringPs1DeditherBoxblurPresetDef() : PresetDef{}
	{
		Name = "ps1-dedither-boxblur";
		Category = "dithering";
	}

	virtual void Build() {
         	ShaderDefs.push_back(DitheringShadersPs1ditherPS1UnditherBoxBlurShaderDef()
.Param("filter_linear", "true")
.Param("scale", "1.0")
.Param("scale_type", "source"));
	}
};
}
