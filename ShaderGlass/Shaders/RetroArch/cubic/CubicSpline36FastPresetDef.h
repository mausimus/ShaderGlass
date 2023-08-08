/*
ShaderGlass preset cubic / spline36-fast imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/6f921ee4815a7894a33855974285b04545a4fa42/cubic/spline36-fast.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class CubicSpline36FastPresetDef : public PresetDef
{
public:
	CubicSpline36FastPresetDef() : PresetDef{}
	{
		Name = "spline36-fast";
		Category = "cubic";
	}

	virtual void Build() {
         	ShaderDefs.push_back(CubicShadersSpline36XShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type_x", "viewport")
.Param("scale_type_y", "source"));
         	ShaderDefs.push_back(CubicShadersSpline36YShaderDef()
.Param("filter_linear", "false"));
	}
};
}
