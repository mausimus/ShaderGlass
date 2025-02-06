/*
ShaderGlass preset pixel-art-scaling / bilinear-adjustable imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/pixel-art-scaling/bilinear-adjustable.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class PixelArtScalingBilinearAdjustablePresetDef : public PresetDef
{
public:
	PixelArtScalingBilinearAdjustablePresetDef() : PresetDef{}
	{
		Name = "bilinear-adjustable";
		Category = "pixel-art-scaling";
	}

	virtual void Build() {
         	ShaderDefs.push_back(PixelArtScalingShadersBilinearAdjustableShaderDef()
.Param("filter_linear", "true")
.Param("scale_type", "viewport"));
	}
};
}
