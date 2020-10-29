/*
ShaderGlass preset interpolation / smuberstep imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/interpolation/smuberstep.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class InterpolationSmuberstepPresetDef : public PresetDef
{
public:
	InterpolationSmuberstepPresetDef() : PresetDef{}
	{
		Name = "smuberstep";
		Category = "interpolation";
	}

	virtual void Build() {
         	ShaderDefs.push_back(InterpolationShadersSmuberStepShaderDef()
.Param("filter_linear", "true"));
	}
};
}
