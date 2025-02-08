/*
ShaderGlass preset presets-tvout / tvout-pixelsharp imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/presets/tvout/tvout-pixelsharp.slangp
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
         	ShaderDefs.push_back(MiscShadersImageAdjustmentShaderDef()
.Param("scale_type_x", "source")
.Param("scale_type_y", "source")
.Param("scale_x", "1.000000")
.Param("scale_y", "4.000000"));
         	ShaderDefs.push_back(StockStockShaderDef()
.Param("filter_linear", "true"));
            OverrideParam("B", (float)1.000000);
            OverrideParam("G", (float)1.000000);
            OverrideParam("R", (float)1.000000);
            OverrideParam("TVOUT_COMPOSITE_CONNECTION", (float)0.000000);
            OverrideParam("TVOUT_RESOLUTION", (float)1024.000000);
            OverrideParam("TVOUT_TV_COLOR_LEVELS", (float)1.000000);
            OverrideParam("bright_boost", (float)0.000000);
            OverrideParam("contrast", (float)1.000000);
            OverrideParam("luminance", (float)1.000000);
            OverrideParam("monitor_gamma", (float)2.200000);
            OverrideParam("overscan_percent_x", (float)0.000000);
            OverrideParam("overscan_percent_y", (float)0.000000);
            OverrideParam("saturation", (float)1.000000);
            OverrideParam("target_gamma", (float)2.400000);
	}
};
}
