/*
ShaderGlass preset edge-smoothing-eagle / 2xsai imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/edge-smoothing/eagle/2xsai.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class EdgeSmoothingEagle2xsaiPresetDef : public PresetDef
{
public:
	EdgeSmoothingEagle2xsaiPresetDef() : PresetDef{}
	{
		Name = "2xsai";
		Category = "edge-smoothing-eagle";
	}

	virtual void Build() {
         	ShaderDefs.push_back(EdgeSmoothingEagleShaders2xsaiShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "source")
.Param("scale_x", "2.0")
.Param("scale_y", "2.0"));
         	ShaderDefs.push_back(InterpolationShadersBicubicXShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type_x", "viewport")
.Param("scale_type_y", "source"));
         	ShaderDefs.push_back(InterpolationShadersBicubicYShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "viewport"));
	}
};
}
