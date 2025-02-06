/*
ShaderGlass preset pixel-art-scaling / sharp-bilinear-scanlines imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/pixel-art-scaling/sharp-bilinear-scanlines.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class PixelArtScalingSharpBilinearScanlinesPresetDef : public PresetDef
{
public:
	PixelArtScalingSharpBilinearScanlinesPresetDef() : PresetDef{}
	{
		Name = "sharp-bilinear-scanlines";
		Category = "pixel-art-scaling";
	}

	virtual void Build() {
         	ShaderDefs.push_back(PixelArtScalingShadersSharpBilinearScanlinesShaderDef()
.Param("filter_linear", "true")
.Param("scale_type", "viewport"));
	}
};
}
