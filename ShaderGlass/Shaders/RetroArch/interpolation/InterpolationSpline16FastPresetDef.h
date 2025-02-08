/*
ShaderGlass preset interpolation / spline16-fast imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/interpolation/spline16-fast.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class InterpolationSpline16FastPresetDef : public PresetDef
{
public:
	InterpolationSpline16FastPresetDef() : PresetDef{}
	{
		Name = "spline16-fast";
		Category = "interpolation";
	}

	virtual void Build() {
         	ShaderDefs.push_back(InterpolationShadersSpline16XShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type_x", "viewport")
.Param("scale_type_y", "source")
.Param("wrap_mode", "clamp_to_edge"));
         	ShaderDefs.push_back(InterpolationShadersSpline16YShaderDef()
.Param("filter_linear", "false")
.Param("wrap_mode", "clamp_to_edge"));
	}
};
}
