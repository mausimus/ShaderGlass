/*
ShaderGlass preset presets-blurs / gizmo-composite-blur-aa-lv2-fsr-gsharp-resampler imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/presets/blurs/gizmo-composite-blur-aa-lv2-fsr-gsharp-resampler.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class PresetsBlursGizmoCompositeBlurAaLv2FsrGsharpResamplerPresetDef : public PresetDef
{
public:
	PresetsBlursGizmoCompositeBlurAaLv2FsrGsharpResamplerPresetDef() : PresetDef{}
	{
		Name = "gizmo-composite-blur-aa-lv2-fsr-gsharp-resampler";
		Category = "presets-blurs";
	}

	virtual void Build() {
         	ShaderDefs.push_back(BlursShadersGizmoBlurShaderDef()
.Param("filter_linear", "true")
.Param("float_framebuffer", "false")
.Param("mipmap_input", "false")
.Param("scale_type_x", "viewport")
.Param("scale_type_y", "viewport")
.Param("scale_x", "1.000000")
.Param("scale_y", "1.000000")
.Param("srgb_framebuffer", "false")
.Param("wrap_mode", "clamp_to_border"));
         	ShaderDefs.push_back(AntiAliasingShadersAaShader40Level2AaShader40Level2Pass1ShaderDef()
.Param("filter_linear", "false")
.Param("float_framebuffer", "false")
.Param("mipmap_input", "false")
.Param("scale_type_x", "source")
.Param("scale_type_y", "source")
.Param("scale_x", "2.000000")
.Param("scale_y", "2.000000")
.Param("srgb_framebuffer", "false")
.Param("wrap_mode", "clamp_to_border"));
         	ShaderDefs.push_back(AntiAliasingShadersAaShader40Level2AaShader40Level2Pass2ShaderDef()
.Param("filter_linear", "false")
.Param("float_framebuffer", "false")
.Param("mipmap_input", "false")
.Param("scale_type_x", "source")
.Param("scale_type_y", "source")
.Param("scale_x", "2.000000")
.Param("scale_y", "2.000000")
.Param("srgb_framebuffer", "false")
.Param("wrap_mode", "clamp_to_border"));
         	ShaderDefs.push_back(EdgeSmoothingFsrShadersFsrPass0ShaderDef()
.Param("filter_linear", "true")
.Param("float_framebuffer", "false")
.Param("mipmap_input", "false")
.Param("scale_type_x", "viewport")
.Param("scale_type_y", "viewport")
.Param("scale_x", "1.000000")
.Param("scale_y", "1.000000")
.Param("srgb_framebuffer", "false")
.Param("wrap_mode", "clamp_to_edge"));
         	ShaderDefs.push_back(EdgeSmoothingFsrShadersFsrPass1ShaderDef()
.Param("filter_linear", "true")
.Param("float_framebuffer", "false")
.Param("mipmap_input", "false")
.Param("scale_type_x", "source")
.Param("scale_type_y", "source")
.Param("scale_x", "1.000000")
.Param("scale_y", "1.000000")
.Param("srgb_framebuffer", "false")
.Param("wrap_mode", "clamp_to_edge"));
         	ShaderDefs.push_back(DitheringShadersGSharp_resamplerShaderDef()
.Param("float_framebuffer", "false")
.Param("mipmap_input", "false")
.Param("scale_type_x", "source")
.Param("scale_type_y", "source")
.Param("scale_x", "1.000000")
.Param("scale_y", "1.000000")
.Param("srgb_framebuffer", "false")
.Param("wrap_mode", "clamp_to_border"));
            OverrideParam("BLUR_OFFSET", (float)1.000000);
            OverrideParam("COLOUR_BLEEDING", (float)3.000000);
            OverrideParam("FSR_SHARPENING", (float)0.300000);
            OverrideParam("HORIZONTAL_BLUR", (float)1.000000);
            OverrideParam("SUBPIXEL_SCALING", (float)1.000000);
            OverrideParam("VERTICAL_BLUR", (float)1.000000);
            OverrideParam("feedback_pass", (float)0.000000);
            OverrideParam("mixer", (float)200.000000);
	}
};
}
