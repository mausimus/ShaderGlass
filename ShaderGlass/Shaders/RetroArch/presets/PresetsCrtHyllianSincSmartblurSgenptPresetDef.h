/*
ShaderGlass preset presets / crt-hyllian-sinc-smartblur-sgenpt imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/presets/crt-hyllian-sinc-smartblur-sgenpt.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class PresetsCrtHyllianSincSmartblurSgenptPresetDef : public PresetDef
{
public:
	PresetsCrtHyllianSincSmartblurSgenptPresetDef() : PresetDef{}
	{
		Name = "crt-hyllian-sinc-smartblur-sgenpt";
		Category = "presets";
	}

	virtual void Build() {
         	ShaderDefs.push_back(DitheringShadersSgenptMixShaderDef()
.Param("filter_linear", "false")
.Param("float_framebuffer", "false")
.Param("mipmap_input", "false")
.Param("scale_type_x", "source")
.Param("scale_type_y", "source")
.Param("scale_x", "2.000000")
.Param("scale_y", "2.000000")
.Param("srgb_framebuffer", "false")
.Param("wrap_mode", "clamp_to_border"));
         	ShaderDefs.push_back(BlursShadersSmartBlurShaderDef()
.Param("filter_linear", "false")
.Param("float_framebuffer", "false")
.Param("mipmap_input", "false")
.Param("scale_type_x", "source")
.Param("scale_type_y", "source")
.Param("scale_x", "1.000000")
.Param("scale_y", "1.000000")
.Param("srgb_framebuffer", "false")
.Param("wrap_mode", "clamp_to_border"));
         	ShaderDefs.push_back(StockStockShaderDef()
.Param("filter_linear", "false")
.Param("float_framebuffer", "false")
.Param("mipmap_input", "false")
.Param("scale_type_x", "source")
.Param("scale_type_y", "source")
.Param("scale_x", "0.500000")
.Param("scale_y", "0.500000")
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
.Param("srgb_framebuffer", "false")
.Param("wrap_mode", "clamp_to_edge"));
            OverrideParam("BRIGHTBOOST", (float)1.500000);
            OverrideParam("PHOSPHOR_LAYOUT", (float)2.000000);
            OverrideParam("SB_BLUE_THRESHOLD", (float)0.200000);
            OverrideParam("SB_BLUR_LEVEL", (float)0.660000);
            OverrideParam("SB_GREEN_THRESHOLD", (float)0.200000);
            OverrideParam("SB_RED_THRESHOLD", (float)0.200000);
            OverrideParam("SGPT_ADJUST_VIEW", (float)0.000000);
            OverrideParam("SGPT_BLEND_LEVEL", (float)0.500000);
            OverrideParam("SGPT_BLEND_OPTION", (float)1.000000);
            OverrideParam("SGPT_LINEAR_GAMMA", (float)1.000000);
	}
};
}
