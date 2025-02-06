/*
ShaderGlass preset presets-crt-ntsc-sharp / tate-svideo-ntsc_x6 imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/presets/crt-ntsc-sharp/tate-svideo-ntsc_x6.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class PresetsCrtNtscSharpTateSvideoNtsc_x6PresetDef : public PresetDef
{
public:
	PresetsCrtNtscSharpTateSvideoNtsc_x6PresetDef() : PresetDef{}
	{
		Name = "tate-svideo-ntsc_x6";
		Category = "presets-crt-ntsc-sharp";
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
.Param("srgb_framebuffer", "false"));
         	ShaderDefs.push_back(NtscShadersNtscAdaptiveTateNtscTatePass1ShaderDef()
.Param("filter_linear", "false")
.Param("float_framebuffer", "true")
.Param("mipmap_input", "false")
.Param("scale_type_x", "source")
.Param("scale_type_y", "source")
.Param("scale_x", "1.000000")
.Param("scale_y", "6.000000")
.Param("srgb_framebuffer", "false"));
         	ShaderDefs.push_back(NtscShadersNtscAdaptiveTateNtscTatePass2ShaderDef()
.Param("filter_linear", "true")
.Param("float_framebuffer", "false")
.Param("mipmap_input", "false")
.Param("scale_type_x", "source")
.Param("scale_type_y", "source")
.Param("scale_x", "1.000000")
.Param("scale_y", "0.333333")
.Param("srgb_framebuffer", "false"));
         	ShaderDefs.push_back(WarpShadersSmartMorphShaderDef()
.Param("float_framebuffer", "false")
.Param("mipmap_input", "false")
.Param("scale_type_x", "source")
.Param("scale_type_y", "source")
.Param("scale_x", "1.000000")
.Param("scale_y", "1.000000")
.Param("srgb_framebuffer", "false"));
         	ShaderDefs.push_back(SharpenShadersFastSharpenShaderDef()
.Param("float_framebuffer", "false")
.Param("mipmap_input", "false")
.Param("scale_type_x", "source")
.Param("scale_type_y", "source")
.Param("scale_x", "1.000000")
.Param("scale_y", "1.000000")
.Param("srgb_framebuffer", "false"));
         	ShaderDefs.push_back(CrtShadersCrtGeomShaderDef()
.Param("float_framebuffer", "false")
.Param("mipmap_input", "false")
.Param("srgb_framebuffer", "false"));
	}
};
}
