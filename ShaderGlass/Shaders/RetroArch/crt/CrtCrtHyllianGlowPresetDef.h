/*
ShaderGlass preset crt / crt-hyllian-glow imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/crt/crt-hyllian-glow.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class CrtCrtHyllianGlowPresetDef : public PresetDef
{
public:
	CrtCrtHyllianGlowPresetDef() : PresetDef{}
	{
		Name = "crt-hyllian-glow";
		Category = "crt";
	}

	virtual void Build() {
         	ShaderDefs.push_back(CrtShadersGlowLinearizeShaderDef()
.Param("filter_linear", "false")
.Param("srgb_framebuffer", "true"));
         	ShaderDefs.push_back(CrtShadersCrtHyllianGlowCrtHyllianShaderDef()
.Param("alias", "CRT_PASS")
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
         	ShaderDefs.push_back(CrtShadersCrtHyllianGlowResolve2ShaderDef()
.Param("filter_linear", "true"));
	}
};
}
