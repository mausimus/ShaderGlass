/*
ShaderGlass preset interpolation / smootheststep imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/interpolation/smootheststep.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class InterpolationSmootheststepPresetDef : public PresetDef
{
public:
	InterpolationSmootheststepPresetDef() : PresetDef{}
	{
		Name = "smootheststep";
		Category = "interpolation";
	}

	virtual void Build() {
         	ShaderDefs.push_back(InterpolationShadersSmootheststepShaderDef()
.Param("filter_linear", "true"));
	}
};
}
