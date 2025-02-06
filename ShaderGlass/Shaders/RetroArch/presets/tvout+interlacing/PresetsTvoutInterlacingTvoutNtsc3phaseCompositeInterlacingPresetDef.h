/*
ShaderGlass preset presets-tvout-interlacing / tvout+ntsc-3phase-composite+interlacing imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/presets/tvout+interlacing/tvout+ntsc-3phase-composite+interlacing.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class PresetsTvoutInterlacingTvoutNtsc3phaseCompositeInterlacingPresetDef : public PresetDef
{
public:
	PresetsTvoutInterlacingTvoutNtsc3phaseCompositeInterlacingPresetDef() : PresetDef{}
	{
		Name = "tvout+ntsc-3phase-composite+interlacing";
		Category = "presets-tvout-interlacing";
	}

	virtual void Build() {
         	ShaderDefs.push_back(NtscShadersMaisterNtscPass1Composite3phaseShaderDef()
.Param("filter_linear", "false")
.Param("float_framebuffer", "true")
.Param("frame_count_mod", "2")
.Param("scale_type_x", "source")
.Param("scale_type_y", "source")
.Param("scale_x", "4.000000")
.Param("scale_y", "1.000000"));
         	ShaderDefs.push_back(NtscShadersMaisterNtscPass23phaseShaderDef()
.Param("filter_linear", "false")
.Param("float_framebuffer", "false")
.Param("scale_type_x", "source")
.Param("scale_type_y", "source")
.Param("scale_x", "0.500000")
.Param("scale_y", "1.000000"));
         	ShaderDefs.push_back(CrtShadersTvoutTweaksShaderDef()
.Param("filter_linear", "false")
.Param("float_framebuffer", "false")
.Param("scale_type_x", "viewport")
.Param("scale_type_y", "source")
.Param("scale_x", "1.000000")
.Param("scale_y", "1.000000"));
         	ShaderDefs.push_back(MiscShadersImageAdjustmentShaderDef()
.Param("float_framebuffer", "false"));
         	ShaderDefs.push_back(MiscShadersInterlacingShaderDef());
	}
};
}
