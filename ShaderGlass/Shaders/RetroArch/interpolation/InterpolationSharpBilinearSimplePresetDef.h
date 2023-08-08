/*
ShaderGlass preset interpolation / sharp-bilinear-simple imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/6f921ee4815a7894a33855974285b04545a4fa42/interpolation/sharp-bilinear-simple.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class InterpolationSharpBilinearSimplePresetDef : public PresetDef
{
public:
	InterpolationSharpBilinearSimplePresetDef() : PresetDef{}
	{
		Name = "sharp-bilinear-simple";
		Category = "interpolation";
	}

	virtual void Build() {
         	ShaderDefs.push_back(InterpolationShadersSharpBilinearSimpleShaderDef()
.Param("filter_linear", "true")
.Param("scale_type", "viewport"));
	}
};
}
