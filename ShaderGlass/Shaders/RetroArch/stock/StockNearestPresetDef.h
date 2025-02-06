/*
ShaderGlass preset stock / nearest imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/nearest.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class StockNearestPresetDef : public PresetDef
{
public:
	StockNearestPresetDef() : PresetDef{}
	{
		Name = "nearest";
		Category = "stock";
	}

	virtual void Build() {
         	ShaderDefs.push_back(StockStockShaderDef()
.Param("filter_linear", "false"));
	}
};
}
