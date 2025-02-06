/*
ShaderGlass preset presets-tvout-interlacing / tvout+ntsc-nes+interlacing imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/presets/tvout+interlacing/tvout+ntsc-nes+interlacing.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class PresetsTvoutInterlacingTvoutNtscNesInterlacingPresetDef : public PresetDef
{
public:
	PresetsTvoutInterlacingTvoutNtscNesInterlacingPresetDef() : PresetDef{}
	{
		Name = "tvout+ntsc-nes+interlacing";
		Category = "presets-tvout-interlacing";
	}

	virtual void Build() {
         	ShaderDefs.push_back(Nes_raw_paletteShadersNesColorDecoderShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(NtscShadersMaisterNtscPass1Composite3phaseShaderDef()
.Param("filter_linear", "false")
.Param("float_framebuffer", "true")
.Param("frame_count_mod", "2")
.Param("scale_type_x", "absolute")
.Param("scale_type_y", "source")
.Param("scale_x", "1024")
.Param("scale_y", "1.0"));
         	ShaderDefs.push_back(NtscShadersMaisterNtscPass23phaseShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "source")
.Param("scale_x", "0.5")
.Param("scale_y", "1.0"));
         	ShaderDefs.push_back(CrtShadersTvoutTweaksShaderDef()
.Param("scale_type_x", "viewport")
.Param("scale_type_y", "source")
.Param("scale_x", "1.000000")
.Param("scale_y", "1.000000"));
         	ShaderDefs.push_back(MiscShadersImageAdjustmentShaderDef());
         	ShaderDefs.push_back(MiscShadersInterlacingShaderDef());
	}
};
}
