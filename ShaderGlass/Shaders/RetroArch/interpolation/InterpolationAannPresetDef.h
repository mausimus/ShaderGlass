/*
ShaderGlass preset interpolation / aann imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/interpolation/aann.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class InterpolationAannPresetDef : public PresetDef
{
public:
	InterpolationAannPresetDef() : PresetDef{}
	{
		Name = "aann";
		Category = "interpolation";
	}

	virtual void Build() {
         	ShaderDefs.push_back(InterpolationShadersAannShaderDef()
.Param("filter_linear", "true"));
	}
};
}
