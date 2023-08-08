/*
ShaderGlass preset hqx / hq2x-halphon imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/6f921ee4815a7894a33855974285b04545a4fa42/hqx/hq2x-halphon.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class HqxHq2xHalphonPresetDef : public PresetDef
{
public:
	HqxHq2xHalphonPresetDef() : PresetDef{}
	{
		Name = "hq2x-halphon";
		Category = "hqx";
	}

	virtual void Build() {
         	ShaderDefs.push_back(StockStockShaderDef()
.Param("alias", "hqx_refpass"));
         	ShaderDefs.push_back(HqxShadersHq2xHalphonShaderDef()
.Param("filter_linear", "false")
.Param("scale", "2.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(StockStockShaderDef());
	}
};
}
