/*
ShaderGlass preset crt / crtglow_gauss_ntsc imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/crt/crtglow_gauss_ntsc.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class CrtCrtglow_gauss_ntscPresetDef : public PresetDef
{
public:
	CrtCrtglow_gauss_ntscPresetDef() : PresetDef{}
	{
		Name = "crtglow_gauss_ntsc";
		Category = "crt";
	}

	virtual void Build() {
         	ShaderDefs.push_back(NtscShadersNtscPass1Composite3phaseShaderDef()
.Param("filter_linear", "false")
.Param("float_framebuffer", "true")
.Param("frame_count_mod", "2")
.Param("scale_type", "source")
.Param("scale_x", "4.0")
.Param("scale_y", "1.0"));
         	ShaderDefs.push_back(NtscShadersNtscPass23phaseLinearShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "source")
.Param("scale_x", "0.5")
.Param("scale_y", "1.0")
.Param("srgb_framebuffer", "true"));
         	ShaderDefs.push_back(CrtShadersGlowGauss_horizShaderDef()
.Param("filter_linear", "false")
.Param("scale_type_x", "viewport")
.Param("scale_type_y", "source")
.Param("srgb_framebuffer", "true"));
         	ShaderDefs.push_back(CrtShadersGlowGauss_vertShaderDef()
.Param("alias", "CRTPass")
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "viewport")
.Param("srgb_framebuffer", "true"));
         	ShaderDefs.push_back(CrtShadersGlowThresholdShaderDef()
.Param("filter_linear", "false")
.Param("srgb_framebuffer", "true"));
         	ShaderDefs.push_back(CrtShadersGlowBlur_horizShaderDef()
.Param("filter_linear", "true")
.Param("mipmap_input", "true")
.Param("scale", "0.25")
.Param("scale_type", "source")
.Param("srgb_framebuffer", "true"));
         	ShaderDefs.push_back(CrtShadersGlowBlur_vertShaderDef()
.Param("filter_linear", "true")
.Param("srgb_framebuffer", "true"));
         	ShaderDefs.push_back(CrtShadersGlowResolveShaderDef()
.Param("filter_linear", "true"));
	}
};
}
