/*
ShaderGlass preset xsal / 2xsal imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/xsal/2xsal.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class Xsal2xsalPresetDef : public PresetDef
{
public:
	Xsal2xsalPresetDef() : PresetDef{}
	{
		Name = "2xsal";
		Category = "xsal";
	}

	virtual void Build() {
         	ShaderDefs.push_back(XsalShaders2xsalShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "source")
.Param("scale_x", "2.0")
.Param("scale_y", "2.0"));
         	ShaderDefs.push_back(StockStockShaderDef()
.Param("filter_linear", "true"));
	}
};
}
