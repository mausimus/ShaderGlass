/*
ShaderGlass preset crt / crtglow_lanczos imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/crt/crtglow_lanczos.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class CrtCrtglow_lanczosPresetDef : public PresetDef
{
public:
	CrtCrtglow_lanczosPresetDef() : PresetDef{}
	{
		Name = "crtglow_lanczos";
		Category = "crt";
	}

	virtual void Build() {
         	ShaderDefs.push_back(CrtShadersGlowLinearizeShaderDef()
.Param("filter_linear", "false")
.Param("srgb_framebuffer", "true"));
         	ShaderDefs.push_back(CrtShadersGlowLanczos_horizShaderDef()
.Param("filter_linear", "false")
.Param("scale_type_x", "viewport")
.Param("scale_type_y", "source")
.Param("scale_x", "1.0")
.Param("scale_y", "1.0")
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
.Param("filter_linear", "true")
.Param("scale_type", "viewport"));
	}
};
}
