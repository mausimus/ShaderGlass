/*
ShaderGlass preset presets/crt-ntsc-sharp / svideo-ntsc_x6 imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/presets/crt-ntsc-sharp/svideo-ntsc_x6.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class PresetsCrtNtscSharpSvideoNtsc_x6PresetDef : public PresetDef
{
public:
	PresetsCrtNtscSharpSvideoNtsc_x6PresetDef() : PresetDef{}
	{
		Name = "svideo-ntsc_x6";
		Category = "presets/crt-ntsc-sharp";
	}

	virtual void Build() {
         	ShaderDefs.push_back(MiscShadersNtscColorsShaderDef()
.Param("filter_linear", "false")
.Param("float_framebuffer", "false")
.Param("mipmap_input", "false")
.Param("scale_type_x", "source")
.Param("scale_type_y", "source")
.Param("scale_x", "1.000000")
.Param("scale_y", "1.000000")
.Param("srgb_framebuffer", "false")
.Param("wrap_mode", "clamp_to_border"));
         	ShaderDefs.push_back(NtscShadersNtscAdaptiveNtscPass1ShaderDef()
.Param("filter_linear", "false")
.Param("float_framebuffer", "true")
.Param("mipmap_input", "false")
.Param("scale_type_x", "source")
.Param("scale_type_y", "source")
.Param("scale_x", "6.000000")
.Param("scale_y", "1.000000")
.Param("srgb_framebuffer", "false")
.Param("wrap_mode", "clamp_to_border"));
         	ShaderDefs.push_back(NtscShadersNtscAdaptiveNtscPass2ShaderDef()
.Param("filter_linear", "true")
.Param("float_framebuffer", "false")
.Param("mipmap_input", "false")
.Param("scale_type_x", "source")
.Param("scale_type_y", "source")
.Param("scale_x", "0.333333")
.Param("scale_y", "1.000000")
.Param("srgb_framebuffer", "false")
.Param("wrap_mode", "clamp_to_border"));
         	ShaderDefs.push_back(WarpShadersSmartMorphShaderDef()
.Param("float_framebuffer", "false")
.Param("mipmap_input", "false")
.Param("scale_type_x", "source")
.Param("scale_type_y", "source")
.Param("scale_x", "1.000000")
.Param("scale_y", "1.000000")
.Param("srgb_framebuffer", "false")
.Param("wrap_mode", "clamp_to_border"));
         	ShaderDefs.push_back(SharpenShadersFastSharpenShaderDef()
.Param("float_framebuffer", "false")
.Param("mipmap_input", "false")
.Param("scale_type_x", "source")
.Param("scale_type_y", "source")
.Param("scale_x", "1.000000")
.Param("scale_y", "1.000000")
.Param("srgb_framebuffer", "false")
.Param("wrap_mode", "clamp_to_border"));
         	ShaderDefs.push_back(CrtShadersCrtGeomShaderDef()
.Param("float_framebuffer", "false")
.Param("mipmap_input", "false")
.Param("srgb_framebuffer", "false")
.Param("wrap_mode", "clamp_to_border"));
            OverrideParam("CONTR", (float)0.150000);
            OverrideParam("CRTgamma", (float)2.200000);
            OverrideParam("CURVATURE", (float)1.000000);
            OverrideParam("DOTMASK", (float)0.300000);
            OverrideParam("R", (float)1.800000);
            OverrideParam("SM_PWR", (float)1.100000);
            OverrideParam("SM_RANGE", (float)1.000000);
            OverrideParam("cornersize", (float)0.015500);
            OverrideParam("cust_artifacting", (float)1.000000);
            OverrideParam("cust_fringing", (float)1.000000);
            OverrideParam("d", (float)1.500000);
            OverrideParam("intensity", (float)0.000000);
            OverrideParam("interlace_detect", (float)0.000000);
            OverrideParam("lum", (float)0.080000);
            OverrideParam("mask_type", (float)10.000000);
            OverrideParam("quality", (float)0.000000);
            OverrideParam("scanline_weight", (float)0.250000);
            OverrideParam("y_tilt", (float)-0.050000);
	}
};
}
