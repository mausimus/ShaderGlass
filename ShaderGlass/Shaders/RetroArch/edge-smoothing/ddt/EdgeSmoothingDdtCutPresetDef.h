/*
ShaderGlass preset edge-smoothing-ddt / cut imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/edge-smoothing/ddt/cut.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class EdgeSmoothingDdtCutPresetDef : public PresetDef
{
public:
	EdgeSmoothingDdtCutPresetDef() : PresetDef{}
	{
		Name = "cut";
		Category = "edge-smoothing-ddt";
	}

	virtual void Build() {
         	ShaderDefs.push_back(EdgeSmoothingDdtShadersCutShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "viewport"));
	}
};
}
