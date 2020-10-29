/*
ShaderGlass preset interpolation / controlled_sharpness imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/interpolation/controlled_sharpness.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class InterpolationControlled_sharpnessPresetDef : public PresetDef
{
public:
	InterpolationControlled_sharpnessPresetDef() : PresetDef{}
	{
		Name = "controlled_sharpness";
		Category = "interpolation";
	}

	virtual void Build() {
         	ShaderDefs.push_back(InterpolationShadersControlledSharpnessShaderDef()
.Param("filter_linear", "true"));
	}
};
}
