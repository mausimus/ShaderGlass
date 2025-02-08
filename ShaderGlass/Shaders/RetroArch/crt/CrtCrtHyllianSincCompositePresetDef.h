/*
ShaderGlass preset crt / crt-hyllian-sinc-composite imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/crt/crt-hyllian-sinc-composite.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class CrtCrtHyllianSincCompositePresetDef : public PresetDef
{
public:
	CrtCrtHyllianSincCompositePresetDef() : PresetDef{}
	{
		Name = "crt-hyllian-sinc-composite";
		Category = "crt";
	}

	virtual void Build() {
         	ShaderDefs.push_back(CrtShadersHyllianSupportMultiLUTModifiedShaderDef()
.Param("float_framebuffer", "false")
.Param("mipmap_input", "false")
.Param("scale_type_x", "source")
.Param("scale_type_y", "source")
.Param("scale_x", "1.000000")
.Param("scale_y", "1.000000")
.Param("srgb_framebuffer", "false")
.Param("wrap_mode", "clamp_to_border"));
         	ShaderDefs.push_back(CrtShadersHyllianCrtHyllianSincPass0ShaderDef()
.Param("filter_linear", "false")
.Param("float_framebuffer", "false")
.Param("mipmap_input", "false")
.Param("scale_type_x", "viewport")
.Param("scale_type_y", "source")
.Param("scale_x", "1.000000")
.Param("scale_y", "1.000000")
.Param("srgb_framebuffer", "true")
.Param("wrap_mode", "clamp_to_border"));
         	ShaderDefs.push_back(CrtShadersHyllianCrtHyllianPass1ShaderDef()
.Param("filter_linear", "false")
.Param("float_framebuffer", "false")
.Param("mipmap_input", "false")
.Param("scale_type_x", "source")
.Param("scale_type_y", "viewport")
.Param("scale_x", "1.000000")
.Param("scale_y", "1.000000")
.Param("srgb_framebuffer", "true")
.Param("wrap_mode", "clamp_to_edge"));
            TextureDefs.push_back(CrtShadersGuestAdvancedLutNtscLutTextureDef()
.Param("linear", "true")
.Param("name", "SamplerLUT1"));
            TextureDefs.push_back(CrtShadersHyllianSupportLUTSomeGradeTextureDef()
.Param("linear", "true")
.Param("name", "SamplerLUT2"));
            OverrideParam("BRIGHTBOOST", (float)1.500000);
            OverrideParam("HFILTER_PROFILE", (float)2.000000);
            OverrideParam("LUT_selector_param", (float)2.000000);
            OverrideParam("PHOSPHOR_LAYOUT", (float)2.000000);
            OverrideParam("feedback_pass", (float)0.000000);
	}
};
}
