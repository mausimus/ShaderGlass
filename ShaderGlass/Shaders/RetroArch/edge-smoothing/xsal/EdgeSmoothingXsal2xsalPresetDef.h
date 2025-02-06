/*
ShaderGlass preset edge-smoothing-xsal / 2xsal imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/edge-smoothing/xsal/2xsal.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class EdgeSmoothingXsal2xsalPresetDef : public PresetDef
{
public:
	EdgeSmoothingXsal2xsalPresetDef() : PresetDef{}
	{
		Name = "2xsal";
		Category = "edge-smoothing-xsal";
	}

	virtual void Build() {
         	ShaderDefs.push_back(EdgeSmoothingXsalShaders2xsalShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "source")
.Param("scale_x", "2.0")
.Param("scale_y", "2.0"));
         	ShaderDefs.push_back(InterpolationShadersSpline16XShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type_x", "viewport")
.Param("scale_type_y", "source"));
         	ShaderDefs.push_back(InterpolationShadersSpline16YShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "viewport"));
	}
};
}
