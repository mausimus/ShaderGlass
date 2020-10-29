/*
ShaderGlass preset xbrz / 5xbrz-linear imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/xbrz/5xbrz-linear.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class Xbrz5xbrzLinearPresetDef : public PresetDef
{
public:
	Xbrz5xbrzLinearPresetDef() : PresetDef{}
	{
		Name = "5xbrz-linear";
		Category = "xbrz";
	}

	virtual void Build() {
         	ShaderDefs.push_back(XbrzShaders5xbrzShaderDef()
.Param("filter_linear", "false")
.Param("scale", "5.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(StockStockShaderDef()
.Param("filter_linear", "true"));
	}
};
}
