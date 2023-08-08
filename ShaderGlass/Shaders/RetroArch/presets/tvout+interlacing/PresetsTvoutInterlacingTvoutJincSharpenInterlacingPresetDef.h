/*
ShaderGlass preset presets-tvout-interlacing / tvout-jinc-sharpen+interlacing imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/6f921ee4815a7894a33855974285b04545a4fa42/presets/tvout+interlacing/tvout-jinc-sharpen+interlacing.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class PresetsTvoutInterlacingTvoutJincSharpenInterlacingPresetDef : public PresetDef
{
public:
	PresetsTvoutInterlacingTvoutJincSharpenInterlacingPresetDef() : PresetDef{}
	{
		Name = "tvout-jinc-sharpen+interlacing";
		Category = "presets-tvout-interlacing";
	}

	virtual void Build() {
         	ShaderDefs.push_back(CrtShadersTvoutTweaksShaderDef()
.Param("scale_type_x", "source")
.Param("scale_type_y", "source")
.Param("scale_x", "2.000000")
.Param("scale_y", "1.000000"));
         	ShaderDefs.push_back(MiscShadersImageAdjustmentShaderDef());
         	ShaderDefs.push_back(WindowedShadersJinc2ShaderDef()
.Param("scale_type_x", "viewport")
.Param("scale_type_y", "source")
.Param("scale_x", "1.000000")
.Param("scale_y", "1.000000"));
         	ShaderDefs.push_back(MiscShadersInterlacingShaderDef());
	}
};
}
