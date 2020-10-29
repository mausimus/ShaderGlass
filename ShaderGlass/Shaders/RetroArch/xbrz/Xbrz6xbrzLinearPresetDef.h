/*
ShaderGlass preset xbrz / 6xbrz-linear imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/xbrz/6xbrz-linear.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class Xbrz6xbrzLinearPresetDef : public PresetDef
{
public:
	Xbrz6xbrzLinearPresetDef() : PresetDef{}
	{
		Name = "6xbrz-linear";
		Category = "xbrz";
	}

	virtual void Build() {
         	ShaderDefs.push_back(XbrzShaders6xbrzShaderDef()
.Param("filter_linear", "false")
.Param("scale", "6.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(StockStockShaderDef()
.Param("filter_linear", "true"));
	}
};
}
