/*
ShaderGlass preset edge-smoothing-xbrz / xbrz-freescale imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/edge-smoothing/xbrz/xbrz-freescale.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class EdgeSmoothingXbrzXbrzFreescalePresetDef : public PresetDef
{
public:
	EdgeSmoothingXbrzXbrzFreescalePresetDef() : PresetDef{}
	{
		Name = "xbrz-freescale";
		Category = "edge-smoothing-xbrz";
	}

	virtual void Build() {
         	ShaderDefs.push_back(EdgeSmoothingXbrzShadersXbrzFreescaleShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "viewport"));
	}
};
}
