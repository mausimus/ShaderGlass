/*
ShaderGlass preset stock / bilinear imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/bilinear.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class StockBilinearPresetDef : public PresetDef
{
public:
	StockBilinearPresetDef() : PresetDef{}
	{
		Name = "bilinear";
		Category = "stock";
	}

	virtual void Build() {
         	ShaderDefs.push_back(StockStockShaderDef()
.Param("filter_linear", "true"));
	}
};
}
