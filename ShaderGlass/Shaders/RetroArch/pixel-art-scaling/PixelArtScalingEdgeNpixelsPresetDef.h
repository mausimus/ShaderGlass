/*
ShaderGlass preset pixel-art-scaling / edgeNpixels imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/pixel-art-scaling/edgeNpixels.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class PixelArtScalingEdgeNpixelsPresetDef : public PresetDef
{
public:
	PixelArtScalingEdgeNpixelsPresetDef() : PresetDef{}
	{
		Name = "edgeNpixels";
		Category = "pixel-art-scaling";
	}

	virtual void Build() {
         	ShaderDefs.push_back(PixelArtScalingShadersEdgeNpixelsShaderDef()
.Param("filter_linear", "true")
.Param("scale_type", "viewport"));
	}
};
}
