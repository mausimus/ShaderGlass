/*
ShaderGlass preset crt / crt-lottes-multipass imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/crt/crt-lottes-multipass.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class CrtCrtLottesMultipassPresetDef : public PresetDef
{
public:
	CrtCrtLottesMultipassPresetDef() : PresetDef{}
	{
		Name = "crt-lottes-multipass";
		Category = "crt";
	}

	virtual void Build() {
         	ShaderDefs.push_back(StockStockShaderDef()
.Param("alias", "Reference"));
         	ShaderDefs.push_back(CrtShadersCrtLottesMultipassBloompassShaderDef()
.Param("alias", "BloomPass")
.Param("filter_linear", "true"));
         	ShaderDefs.push_back(CrtShadersCrtLottesMultipassScanpassShaderDef()
.Param("filter_linear", "true")
.Param("scale_type", "viewport"));
	}
};
}
