/*
ShaderGlass preset xsal / 4xsal-level2-hq imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/xsal/4xsal-level2-hq.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class Xsal4xsalLevel2HqPresetDef : public PresetDef
{
public:
	Xsal4xsalLevel2HqPresetDef() : PresetDef{}
	{
		Name = "4xsal-level2-hq";
		Category = "xsal";
	}

	virtual void Build() {
         	ShaderDefs.push_back(XsalShaders2xsalLevel2HqShaderDef()
.Param("filter_linear", "false")
.Param("scale", "4.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(XsalShaders2xsalLevel2Pass2ShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(StockStockShaderDef()
.Param("filter_linear", "true"));
	}
};
}
