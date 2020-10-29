/*
ShaderGlass preset interpolation / sharp-bilinear-scanlines imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/interpolation/sharp-bilinear-scanlines.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class InterpolationSharpBilinearScanlinesPresetDef : public PresetDef
{
public:
	InterpolationSharpBilinearScanlinesPresetDef() : PresetDef{}
	{
		Name = "sharp-bilinear-scanlines";
		Category = "interpolation";
	}

	virtual void Build() {
         	ShaderDefs.push_back(InterpolationShadersSharpBilinearScanlinesShaderDef()
.Param("filter_linear", "true"));
	}
};
}
