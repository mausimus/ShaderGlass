/*
ShaderGlass preset pixel-art-scaling / sharp-bilinear imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/pixel-art-scaling/sharp-bilinear.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class PixelArtScalingSharpBilinearPresetDef : public PresetDef
{
public:
	PixelArtScalingSharpBilinearPresetDef() : PresetDef{}
	{
		Name = "sharp-bilinear";
		Category = "pixel-art-scaling";
	}

	virtual void Build() {
         	ShaderDefs.push_back(PixelArtScalingShadersSharpBilinearShaderDef()
.Param("filter_linear", "true")
.Param("scale_type", "viewport"));
	}
};
}
