/*
ShaderGlass preset scalenx / scale2xplus imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/scalenx/scale2xplus.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class ScalenxScale2xplusPresetDef : public PresetDef
{
public:
	ScalenxScale2xplusPresetDef() : PresetDef{}
	{
		Name = "scale2xplus";
		Category = "scalenx";
	}

	virtual void Build() {
         	ShaderDefs.push_back(ScalenxShadersScale2xplusShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "source")
.Param("scale_x", "2.0")
.Param("scale_y", "2.0"));
         	ShaderDefs.push_back(StockStockShaderDef()
.Param("filter_linear", "true"));
	}
};
}
