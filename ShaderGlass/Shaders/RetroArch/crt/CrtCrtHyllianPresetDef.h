/*
ShaderGlass preset crt / crt-hyllian imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/crt/crt-hyllian.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class CrtCrtHyllianPresetDef : public PresetDef
{
public:
	CrtCrtHyllianPresetDef() : PresetDef{}
	{
		Name = "crt-hyllian";
		Category = "crt";
	}

	virtual void Build() {
         	ShaderDefs.push_back(CrtShadersHyllianSupportMultiLUTLinearShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.000000")
.Param("scale_type", "source")
.Param("srgb_framebuffer", "true"));
         	ShaderDefs.push_back(CrtShadersHyllianCrtHyllianBaseShaderDef()
.Param("alias", "CRTPass")
.Param("filter_linear", "false")
.Param("scale", "1.000000")
.Param("scale_type", "viewport")
.Param("srgb_framebuffer", "true"));
         	ShaderDefs.push_back(CrtShadersHyllianSupportGlowThresholdShaderDef()
.Param("filter_linear", "true")
.Param("srgb_framebuffer", "true"));
         	ShaderDefs.push_back(CrtShadersHyllianSupportGlowBlur_horizShaderDef()
.Param("filter_linear", "true")
.Param("mipmap_input", "true")
.Param("scale_type_x", "viewport")
.Param("scale_type_y", "viewport")
.Param("scale_x", "0.200000")
.Param("scale_y", "1.000000")
.Param("srgb_framebuffer", "true"));
         	ShaderDefs.push_back(CrtShadersHyllianSupportGlowBlurGlowMaskGeomShaderDef()
.Param("filter_linear", "true")
.Param("scale_type", "viewport")
.Param("srgb_framebuffer", "true")
.Param("wrap_mode", "clamp_to_border"));
            TextureDefs.push_back(ReshadeShadersLUTGradeRgbTextureDef()
.Param("linear", "true")
.Param("name", "SamplerLUT1"));
            TextureDefs.push_back(ReshadeShadersLUTGradeCompositeTextureDef()
.Param("linear", "true")
.Param("name", "SamplerLUT2"));
	}
};
}
