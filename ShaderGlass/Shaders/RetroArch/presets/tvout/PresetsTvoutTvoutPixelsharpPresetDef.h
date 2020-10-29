/*
ShaderGlass preset presets-tvout / tvout-pixelsharp imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/presets/tvout/tvout-pixelsharp.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class PresetsTvoutTvoutPixelsharpPresetDef : public PresetDef
{
public:
	PresetsTvoutTvoutPixelsharpPresetDef() : PresetDef{}
	{
		Name = "tvout-pixelsharp";
		Category = "presets-tvout";
	}

	virtual void Build() {
         	ShaderDefs.push_back(CrtShadersTvoutTweaksShaderDef()
.Param("scale_type_x", "viewport")
.Param("scale_type_y", "source")
.Param("scale_x", "1.000000")
.Param("scale_y", "1.000000"));
         	ShaderDefs.push_back(MiscImageAdjustmentShaderDef()
.Param("scale_type_x", "source")
.Param("scale_type_y", "source")
.Param("scale_x", "1.000000")
.Param("scale_y", "4.000000"));
         	ShaderDefs.push_back(StockStockShaderDef()
.Param("filter_linear", "true"));
	}
};
}
