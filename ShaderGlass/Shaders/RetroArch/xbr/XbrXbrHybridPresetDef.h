/*
ShaderGlass preset xbr / xbr-hybrid imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/xbr/xbr-hybrid.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class XbrXbrHybridPresetDef : public PresetDef
{
public:
	XbrXbrHybridPresetDef() : PresetDef{}
	{
		Name = "xbr-hybrid";
		Category = "xbr";
	}

	virtual void Build() {
         	ShaderDefs.push_back(XbrShadersXbrHybrid2xbrHybridV5GammaShaderDef()
.Param("filter_linear", "false")
.Param("scale", "2.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(XbrShadersXbrHybrid2xbrHybridV5GammaShaderDef()
.Param("filter_linear", "false")
.Param("scale", "2.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(StockStockShaderDef()
.Param("filter_linear", "true"));
	}
};
}
