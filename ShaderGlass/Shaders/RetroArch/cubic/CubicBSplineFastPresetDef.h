/*
ShaderGlass preset cubic / b-spline-fast imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/cubic/b-spline-fast.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class CubicBSplineFastPresetDef : public PresetDef
{
public:
	CubicBSplineFastPresetDef() : PresetDef{}
	{
		Name = "b-spline-fast";
		Category = "cubic";
	}

	virtual void Build() {
         	ShaderDefs.push_back(CubicShadersBSplineFastShaderDef()
.Param("filter_linear", "true"));
	}
};
}
