/*
ShaderGlass preset pixel-art-scaling / sharp-shimmerless imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/pixel-art-scaling/sharp-shimmerless.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class PixelArtScalingSharpShimmerlessPresetDef : public PresetDef
{
public:
	PixelArtScalingSharpShimmerlessPresetDef() : PresetDef{}
	{
		Name = "sharp-shimmerless";
		Category = "pixel-art-scaling";
	}

	virtual void Build() {
         	ShaderDefs.push_back(PixelArtScalingShadersSharpShimmerlessShaderDef()
.Param("filter_linear", "true"));
	}
};
}
