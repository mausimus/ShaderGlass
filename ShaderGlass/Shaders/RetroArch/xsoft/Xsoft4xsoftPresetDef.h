/*
ShaderGlass preset xsoft / 4xsoft imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/xsoft/4xsoft.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class Xsoft4xsoftPresetDef : public PresetDef
{
public:
	Xsoft4xsoftPresetDef() : PresetDef{}
	{
		Name = "4xsoft";
		Category = "xsoft";
	}

	virtual void Build() {
         	ShaderDefs.push_back(StockStockShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "source")
.Param("scale_x", "2.0")
.Param("scale_y", "2.0"));
         	ShaderDefs.push_back(XsoftShaders4xsoftShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "source")
.Param("scale_x", "2.0")
.Param("scale_y", "2.0"));
         	ShaderDefs.push_back(StockStockShaderDef()
.Param("filter_linear", "true"));
	}
};
}
