/*
ShaderGlass preset presets/tvout-interlacing / tvout+ntsc-3phase-svideo+interlacing imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/presets/tvout+interlacing/tvout+ntsc-3phase-svideo+interlacing.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class PresetsTvoutInterlacingTvoutNtsc3phaseSvideoInterlacingPresetDef : public PresetDef
{
public:
	PresetsTvoutInterlacingTvoutNtsc3phaseSvideoInterlacingPresetDef() : PresetDef{}
	{
		Name = "tvout+ntsc-3phase-svideo+interlacing";
		Category = "presets/tvout-interlacing";
	}

	virtual void Build() {
         	ShaderDefs.push_back(NtscShadersMaisterNtscPass1Svideo3phaseShaderDef()
.Param("filter_linear", "false")
.Param("float_framebuffer", "true")
.Param("frame_count_mod", "2")
.Param("scale_type_x", "source")
.Param("scale_type_y", "source")
.Param("scale_x", "6.000000")
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
            OverrideParam("B", (float)1.000000);
            OverrideParam("G", (float)1.000000);
            OverrideParam("R", (float)1.000000);
            OverrideParam("TVOUT_COMPOSITE_CONNECTION", (float)0.000000);
            OverrideParam("TVOUT_RESOLUTION", (float)512.000000);
            OverrideParam("TVOUT_TV_COLOR_LEVELS", (float)1.000000);
            OverrideParam("bright_boost", (float)0.000000);
            OverrideParam("contrast", (float)1.000000);
            OverrideParam("luminance", (float)1.000000);
            OverrideParam("monitor_gamma", (float)2.200000);
            OverrideParam("overscan_percent_x", (float)0.000000);
            OverrideParam("overscan_percent_y", (float)0.000000);
            OverrideParam("percent", (float)0.000000);
            OverrideParam("saturation", (float)1.000000);
            OverrideParam("target_gamma", (float)2.400000);
	}
};
}
