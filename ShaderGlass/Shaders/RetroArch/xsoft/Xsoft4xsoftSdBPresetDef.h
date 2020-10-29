/*
ShaderGlass preset xsoft / 4xsoftSdB imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/xsoft/4xsoftSdB.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class Xsoft4xsoftSdBPresetDef : public PresetDef
{
public:
	Xsoft4xsoftSdBPresetDef() : PresetDef{}
	{
		Name = "4xsoftSdB";
		Category = "xsoft";
	}

	virtual void Build() {
         	ShaderDefs.push_back(StockStockShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "source")
.Param("scale_x", "2.0")
.Param("scale_y", "2.0"));
         	ShaderDefs.push_back(XsoftShaders4xsoftSdBShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "source")
.Param("scale_x", "2.0")
.Param("scale_y", "2.0"));
         	ShaderDefs.push_back(StockStockShaderDef()
.Param("filter_linear", "true"));
	}
};
}
