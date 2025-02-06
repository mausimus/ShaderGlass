/*
ShaderGlass preset edge-smoothing-sabr / sabr-hybrid-deposterize imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/edge-smoothing/sabr/sabr-hybrid-deposterize.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class EdgeSmoothingSabrSabrHybridDeposterizePresetDef : public PresetDef
{
public:
	EdgeSmoothingSabrSabrHybridDeposterizePresetDef() : PresetDef{}
	{
		Name = "sabr-hybrid-deposterize";
		Category = "edge-smoothing-sabr";
	}

	virtual void Build() {
         	ShaderDefs.push_back(EdgeSmoothingSabrShadersSabrHybridDeposterizeShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "viewport"));
	}
};
}
