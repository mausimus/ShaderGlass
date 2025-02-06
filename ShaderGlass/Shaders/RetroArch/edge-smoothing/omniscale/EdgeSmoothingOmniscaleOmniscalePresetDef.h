/*
ShaderGlass preset edge-smoothing-omniscale / omniscale imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/edge-smoothing/omniscale/omniscale.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class EdgeSmoothingOmniscaleOmniscalePresetDef : public PresetDef
{
public:
	EdgeSmoothingOmniscaleOmniscalePresetDef() : PresetDef{}
	{
		Name = "omniscale";
		Category = "edge-smoothing-omniscale";
	}

	virtual void Build() {
         	ShaderDefs.push_back(EdgeSmoothingOmniscaleShadersOmniscaleShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "viewport"));
	}
};
}
