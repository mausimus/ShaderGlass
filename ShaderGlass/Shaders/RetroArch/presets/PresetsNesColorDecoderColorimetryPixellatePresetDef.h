/*
ShaderGlass preset presets / nes-color-decoder+colorimetry+pixellate imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/presets/nes-color-decoder+colorimetry+pixellate.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class PresetsNesColorDecoderColorimetryPixellatePresetDef : public PresetDef
{
public:
	PresetsNesColorDecoderColorimetryPixellatePresetDef() : PresetDef{}
	{
		Name = "nes-color-decoder+colorimetry+pixellate";
		Category = "presets";
	}

	virtual void Build() {
         	ShaderDefs.push_back(Nes_raw_paletteShadersNesColorDecoderShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.000000")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(MiscColorimetryShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.000000")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(InterpolationShadersPixellateShaderDef());
	}
};
}
