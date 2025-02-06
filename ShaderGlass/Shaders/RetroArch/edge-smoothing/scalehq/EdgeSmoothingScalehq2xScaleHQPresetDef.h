/*
ShaderGlass preset edge-smoothing-scalehq / 2xScaleHQ imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/edge-smoothing/scalehq/2xScaleHQ.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class EdgeSmoothingScalehq2xScaleHQPresetDef : public PresetDef
{
public:
	EdgeSmoothingScalehq2xScaleHQPresetDef() : PresetDef{}
	{
		Name = "2xScaleHQ";
		Category = "edge-smoothing-scalehq";
	}

	virtual void Build() {
         	ShaderDefs.push_back(EdgeSmoothingScalehqShaders2xScaleHQShaderDef()
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
