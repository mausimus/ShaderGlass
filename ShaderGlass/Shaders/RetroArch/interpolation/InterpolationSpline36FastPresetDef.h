/*
ShaderGlass preset interpolation / spline36-fast imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/interpolation/spline36-fast.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class InterpolationSpline36FastPresetDef : public PresetDef
{
public:
	InterpolationSpline36FastPresetDef() : PresetDef{}
	{
		Name = "spline36-fast";
		Category = "interpolation";
	}

	virtual void Build() {
         	ShaderDefs.push_back(InterpolationShadersSpline36XShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type_x", "viewport")
.Param("scale_type_y", "source"));
         	ShaderDefs.push_back(InterpolationShadersSpline36YShaderDef()
.Param("filter_linear", "false"));
	}
};
}
