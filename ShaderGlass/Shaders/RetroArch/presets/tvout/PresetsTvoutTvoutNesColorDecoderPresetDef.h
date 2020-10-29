/*
ShaderGlass preset presets-tvout / tvout+nes-color-decoder imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/presets/tvout/tvout+nes-color-decoder.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class PresetsTvoutTvoutNesColorDecoderPresetDef : public PresetDef
{
public:
	PresetsTvoutTvoutNesColorDecoderPresetDef() : PresetDef{}
	{
		Name = "tvout+nes-color-decoder";
		Category = "presets-tvout";
	}

	virtual void Build() {
         	ShaderDefs.push_back(Nes_raw_paletteShadersNesColorDecoderShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(CrtShadersTvoutTweaksShaderDef()
.Param("scale_type_x", "viewport")
.Param("scale_type_y", "source")
.Param("scale_x", "1.000000")
.Param("scale_y", "1.000000"));
         	ShaderDefs.push_back(MiscImageAdjustmentShaderDef());
	}
};
}
