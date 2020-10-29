/*
ShaderGlass preset nes_raw_palette / ntsc-nes imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/nes_raw_palette/ntsc-nes.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class Nes_raw_paletteNtscNesPresetDef : public PresetDef
{
public:
	Nes_raw_paletteNtscNesPresetDef() : PresetDef{}
	{
		Name = "ntsc-nes";
		Category = "nes_raw_palette";
	}

	virtual void Build() {
         	ShaderDefs.push_back(Nes_raw_paletteShadersNesColorDecoderShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(NtscShadersNtscPass1Composite3phaseShaderDef()
.Param("filter_linear", "false")
.Param("float_framebuffer", "true")
.Param("frame_count_mod", "2")
.Param("scale_type_x", "absolute")
.Param("scale_type_y", "source")
.Param("scale_x", "1024")
.Param("scale_y", "1.0"));
         	ShaderDefs.push_back(NtscShadersNtscPass23phaseShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "source")
.Param("scale_x", "0.5")
.Param("scale_y", "1.0"));
	}
};
}
