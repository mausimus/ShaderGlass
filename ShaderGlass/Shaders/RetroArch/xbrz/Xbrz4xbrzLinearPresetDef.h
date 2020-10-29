/*
ShaderGlass preset xbrz / 4xbrz-linear imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/xbrz/4xbrz-linear.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class Xbrz4xbrzLinearPresetDef : public PresetDef
{
public:
	Xbrz4xbrzLinearPresetDef() : PresetDef{}
	{
		Name = "4xbrz-linear";
		Category = "xbrz";
	}

	virtual void Build() {
         	ShaderDefs.push_back(XbrzShaders4xbrzShaderDef()
.Param("filter_linear", "false")
.Param("scale", "4.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(StockStockShaderDef()
.Param("filter_linear", "true"));
	}
};
}
