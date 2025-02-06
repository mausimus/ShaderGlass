/*
ShaderGlass preset edge-smoothing-xbrz / xbrz-freescale-multipass imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/edge-smoothing/xbrz/xbrz-freescale-multipass.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class EdgeSmoothingXbrzXbrzFreescaleMultipassPresetDef : public PresetDef
{
public:
	EdgeSmoothingXbrzXbrzFreescaleMultipassPresetDef() : PresetDef{}
	{
		Name = "xbrz-freescale-multipass";
		Category = "edge-smoothing-xbrz";
	}

	virtual void Build() {
         	ShaderDefs.push_back(StockStockShaderDef()
.Param("alias", "xbrz_fs_refpass"));
         	ShaderDefs.push_back(EdgeSmoothingXbrzShadersXbrzFreescaleMultipassXbrzFreescalePass0ShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(EdgeSmoothingXbrzShadersXbrzFreescaleMultipassXbrzFreescalePass1ShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "viewport"));
	}
};
}
