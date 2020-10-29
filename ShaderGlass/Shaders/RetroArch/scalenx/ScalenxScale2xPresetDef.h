/*
ShaderGlass preset scalenx / scale2x imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/scalenx/scale2x.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class ScalenxScale2xPresetDef : public PresetDef
{
public:
	ScalenxScale2xPresetDef() : PresetDef{}
	{
		Name = "scale2x";
		Category = "scalenx";
	}

	virtual void Build() {
         	ShaderDefs.push_back(ScalenxShadersScale2xShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "source")
.Param("scale_x", "2.0")
.Param("scale_y", "2.0"));
         	ShaderDefs.push_back(StockStockShaderDef()
.Param("filter_linear", "true"));
	}
};
}
