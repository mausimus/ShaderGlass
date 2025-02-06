/*
ShaderGlass preset edge-smoothing-ddt / ddt imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/edge-smoothing/ddt/ddt.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class EdgeSmoothingDdtDdtPresetDef : public PresetDef
{
public:
	EdgeSmoothingDdtDdtPresetDef() : PresetDef{}
	{
		Name = "ddt";
		Category = "edge-smoothing-ddt";
	}

	virtual void Build() {
         	ShaderDefs.push_back(EdgeSmoothingDdtShadersDdtShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "viewport"));
	}
};
}
