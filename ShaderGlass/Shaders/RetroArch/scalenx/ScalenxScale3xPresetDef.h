/*
ShaderGlass preset scalenx / scale3x imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/scalenx/scale3x.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class ScalenxScale3xPresetDef : public PresetDef
{
public:
	ScalenxScale3xPresetDef() : PresetDef{}
	{
		Name = "scale3x";
		Category = "scalenx";
	}

	virtual void Build() {
         	ShaderDefs.push_back(ScalenxShadersScale3xShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "source")
.Param("scale_x", "3.0")
.Param("scale_y", "3.0"));
         	ShaderDefs.push_back(StockStockShaderDef()
.Param("filter_linear", "true"));
	}
};
}
