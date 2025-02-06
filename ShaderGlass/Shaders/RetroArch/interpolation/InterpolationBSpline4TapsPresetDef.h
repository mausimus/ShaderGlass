/*
ShaderGlass preset interpolation / b-spline-4-taps imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/interpolation/b-spline-4-taps.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class InterpolationBSpline4TapsPresetDef : public PresetDef
{
public:
	InterpolationBSpline4TapsPresetDef() : PresetDef{}
	{
		Name = "b-spline-4-taps";
		Category = "interpolation";
	}

	virtual void Build() {
         	ShaderDefs.push_back(InterpolationShadersBSpline4TapsShaderDef()
.Param("filter_linear", "true")
.Param("scale_type", "viewport"));
	}
};
}
