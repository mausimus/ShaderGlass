/*
ShaderGlass preset crt / crt-hyllian-ntsc-rainbow imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/crt/crt-hyllian-ntsc-rainbow.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class CrtCrtHyllianNtscRainbowPresetDef : public PresetDef
{
public:
	CrtCrtHyllianNtscRainbowPresetDef() : PresetDef{}
	{
		Name = "crt-hyllian-ntsc-rainbow";
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
         	ShaderDefs.push_back(CrtShadersHyllianSupportNtscShadersNtscAdaptiveLiteNtscLitePass1ShaderDef()
.Param("filter_linear", "false")
.Param("float_framebuffer", "true")
.Param("mipmap_input", "false")
.Param("scale_type_x", "source")
.Param("scale_type_y", "source")
.Param("scale_x", "4.000000")
.Param("scale_y", "1.000000")
.Param("srgb_framebuffer", "false")
.Param("wrap_mode", "clamp_to_edge"));
         	ShaderDefs.push_back(CrtShadersHyllianSupportNtscShadersNtscAdaptiveLiteNtscLitePass2ShaderDef()
.Param("filter_linear", "true")
.Param("float_framebuffer", "false")
.Param("mipmap_input", "false")
.Param("scale_type_x", "source")
.Param("scale_type_y", "source")
.Param("scale_x", "1.000000")
.Param("scale_y", "1.000000")
.Param("srgb_framebuffer", "false")
.Param("wrap_mode", "clamp_to_edge"));
         	ShaderDefs.push_back(CrtShadersHyllianSupportLinearizeShaderDef()
.Param("filter_linear", "true")
.Param("float_framebuffer", "false")
.Param("mipmap_input", "false")
.Param("scale_type_x", "viewport")
.Param("scale_type_y", "source")
.Param("scale_x", "1.000000")
.Param("scale_y", "1.000000")
.Param("srgb_framebuffer", "true")
.Param("wrap_mode", "clamp_to_edge"));
         	ShaderDefs.push_back(CrtShadersHyllianCrtHyllianPass1ShaderDef()
.Param("filter_linear", "false")
.Param("float_framebuffer", "false")
.Param("mipmap_input", "false")
.Param("scale_type_x", "viewport")
.Param("scale_type_y", "viewport")
.Param("scale_x", "1.000000")
.Param("scale_y", "1.000000")
.Param("srgb_framebuffer", "false")
.Param("wrap_mode", "clamp_to_edge"));
            TextureDefs.push_back(CrtShadersGuestAdvancedLutNtscLutTextureDef()
.Param("linear", "true")
.Param("name", "SamplerLUT1"));
            TextureDefs.push_back(CrtShadersHyllianSupportLUTSomeGradeTextureDef()
.Param("linear", "true")
.Param("name", "SamplerLUT2"));
            OverrideParam("BEAM_MIN_WIDTH", (float)0.860000);
            OverrideParam("BRIGHTBOOST", (float)1.600000);
            OverrideParam("HFILTER_PROFILE", (float)0.000000);
            OverrideParam("InputGamma", (float)2.400000);
            OverrideParam("LUT_selector_param", (float)2.000000);
            OverrideParam("PHOSPHOR_LAYOUT", (float)2.000000);
            OverrideParam("chroma_scale", (float)3.000000);
            OverrideParam("cust_artifacting", (float)0.500000);
            OverrideParam("feedback_pass", (float)0.000000);
            OverrideParam("ntsc_artifacting_rainbow", (float)1.000000);
            OverrideParam("quality", (float)-1.000000);
	}
};
}
