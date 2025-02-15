/*
ShaderGlass preset edge-smoothing/sabr / sabr imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/edge-smoothing/sabr/sabr.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class EdgeSmoothingSabrSabrPresetDef : public PresetDef
{
public:
	EdgeSmoothingSabrSabrPresetDef() : PresetDef{}
	{
		Name = "sabr";
		Category = "edge-smoothing/sabr";
	}

	virtual void Build() {
         	ShaderDefs.push_back(EdgeSmoothingSabrShadersSabrV30ShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "viewport"));
	}
};
}
