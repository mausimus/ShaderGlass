/*
ShaderGlass preset interpolation / pixel_aa imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/6f921ee4815a7894a33855974285b04545a4fa42/interpolation/pixel_aa.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class InterpolationPixel_aaPresetDef : public PresetDef
{
public:
	InterpolationPixel_aaPresetDef() : PresetDef{}
	{
		Name = "pixel_aa";
		Category = "interpolation";
	}

	virtual void Build() {
         	ShaderDefs.push_back(InterpolationShadersPixel_aaPixel_aaShaderDef()
.Param("filter_linear", "true")
.Param("scale_type", "viewport"));
	}
};
}
