/*
ShaderGlass preset anti-aliasing / advanced-aa imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/anti-aliasing/advanced-aa.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class AntiAliasingAdvancedAaPresetDef : public PresetDef
{
public:
	AntiAliasingAdvancedAaPresetDef() : PresetDef{}
	{
		Name = "advanced-aa";
		Category = "anti-aliasing";
	}

	virtual void Build() {
         	ShaderDefs.push_back(AntiAliasingShadersAdvancedAaShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "source")
.Param("scale_x", "2.0")
.Param("scale_y", "2.0"));
         	ShaderDefs.push_back(StockStockShaderDef()
.Param("filter_linear", "true"));
	}
};
}
