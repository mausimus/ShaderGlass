/*
ShaderGlass preset presets / crt-hyllian-curvature-ntsc imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/presets/crt-hyllian-curvature-ntsc.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class PresetsCrtHyllianCurvatureNtscPresetDef : public PresetDef
{
public:
	PresetsCrtHyllianCurvatureNtscPresetDef() : PresetDef{}
	{
		Name = "crt-hyllian-curvature-ntsc";
		Category = "presets";
	}

	virtual void Build() {
         	ShaderDefs.push_back(NtscShadersNtscAdaptiveNtscPass1ShaderDef()
.Param("filter_linear", "false")
.Param("float_framebuffer", "true")
.Param("frame_count_mod", "2")
.Param("mipmap_input", "false")
.Param("scale_type_x", "source")
.Param("scale_type_y", "source")
.Param("scale_x", "4.000000")
.Param("scale_y", "1.000000")
.Param("srgb_framebuffer", "false"));
         	ShaderDefs.push_back(NtscShadersNtscAdaptiveNtscPass2ShaderDef()
.Param("filter_linear", "false")
.Param("float_framebuffer", "false")
.Param("mipmap_input", "false")
.Param("scale_type_x", "source")
.Param("scale_type_y", "source")
.Param("scale_x", "0.500000")
.Param("scale_y", "1.000000")
.Param("srgb_framebuffer", "true"));
         	ShaderDefs.push_back(CrtShadersHyllianCrtHyllianCurvatureShaderDef()
.Param("alias", "CRTPass")
.Param("filter_linear", "false")
.Param("float_framebuffer", "false")
.Param("mipmap_input", "false")
.Param("scale_type_x", "viewport")
.Param("scale_type_y", "viewport")
.Param("scale_x", "1.000000")
.Param("scale_y", "1.000000")
.Param("srgb_framebuffer", "true"));
         	ShaderDefs.push_back(CrtShadersGlowThresholdShaderDef()
.Param("filter_linear", "false")
.Param("float_framebuffer", "false")
.Param("mipmap_input", "false")
.Param("srgb_framebuffer", "true"));
         	ShaderDefs.push_back(CrtShadersGlowBlur_horizShaderDef()
.Param("filter_linear", "true")
.Param("float_framebuffer", "false")
.Param("mipmap_input", "true")
.Param("scale_type_x", "viewport")
.Param("scale_type_y", "viewport")
.Param("scale_x", "0.200000")
.Param("scale_y", "0.200000")
.Param("srgb_framebuffer", "true"));
         	ShaderDefs.push_back(CrtShadersGlowBlur_vertShaderDef()
.Param("filter_linear", "true")
.Param("float_framebuffer", "false")
.Param("mipmap_input", "false")
.Param("srgb_framebuffer", "true"));
         	ShaderDefs.push_back(CrtShadersGlowResolveShaderDef()
.Param("filter_linear", "true")
.Param("float_framebuffer", "false")
.Param("mipmap_input", "false")
.Param("srgb_framebuffer", "false"));
	}
};
}
