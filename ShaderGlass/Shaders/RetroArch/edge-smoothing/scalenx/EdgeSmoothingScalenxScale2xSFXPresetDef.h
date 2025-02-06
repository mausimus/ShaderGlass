/*
ShaderGlass preset edge-smoothing-scalenx / scale2xSFX imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/edge-smoothing/scalenx/scale2xSFX.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class EdgeSmoothingScalenxScale2xSFXPresetDef : public PresetDef
{
public:
	EdgeSmoothingScalenxScale2xSFXPresetDef() : PresetDef{}
	{
		Name = "scale2xSFX";
		Category = "edge-smoothing-scalenx";
	}

	virtual void Build() {
         	ShaderDefs.push_back(EdgeSmoothingScalenxShadersScale2xSFXShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "source")
.Param("scale_x", "2.0")
.Param("scale_y", "2.0"));
         	ShaderDefs.push_back(InterpolationShadersBicubicShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "viewport"));
	}
};
}
