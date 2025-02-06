/*
ShaderGlass preset edge-smoothing-xbrz / 2xbrz-linear imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/edge-smoothing/xbrz/2xbrz-linear.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class EdgeSmoothingXbrz2xbrzLinearPresetDef : public PresetDef
{
public:
	EdgeSmoothingXbrz2xbrzLinearPresetDef() : PresetDef{}
	{
		Name = "2xbrz-linear";
		Category = "edge-smoothing-xbrz";
	}

	virtual void Build() {
         	ShaderDefs.push_back(EdgeSmoothingXbrzShaders2xbrzShaderDef()
.Param("filter_linear", "false")
.Param("scale", "2.0")
.Param("scale_type", "source"));
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
