/*
ShaderGlass preset pixel-art-scaling / pixel_aa_fast imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/pixel-art-scaling/pixel_aa_fast.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class PixelArtScalingPixel_aa_fastPresetDef : public PresetDef
{
public:
	PixelArtScalingPixel_aa_fastPresetDef() : PresetDef{}
	{
		Name = "pixel_aa_fast";
		Category = "pixel-art-scaling";
	}

	virtual void Build() {
         	ShaderDefs.push_back(PixelArtScalingShadersPixel_aaTo_lin_fastShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "source")
.Param("srgb_framebuffer", "true"));
         	ShaderDefs.push_back(PixelArtScalingShadersPixel_aaPixel_aa_fastShaderDef()
.Param("filter_linear", "true")
.Param("scale_type", "viewport"));
	}
};
}
