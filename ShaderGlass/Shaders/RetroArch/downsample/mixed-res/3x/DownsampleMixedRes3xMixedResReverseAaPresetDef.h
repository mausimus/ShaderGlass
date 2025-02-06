/*
ShaderGlass preset downsample-mixed-res-3x / mixed-res-reverse-aa imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/downsample/mixed-res/3x/mixed-res-reverse-aa.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class DownsampleMixedRes3xMixedResReverseAaPresetDef : public PresetDef
{
public:
	DownsampleMixedRes3xMixedResReverseAaPresetDef() : PresetDef{}
	{
		Name = "mixed-res-reverse-aa";
		Category = "downsample-mixed-res-3x";
	}

	virtual void Build() {
         	ShaderDefs.push_back(DownsampleShadersMixedResCoderShaderDef()
.Param("scale", "1.000000")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(DownsampleShadersMixedResHiresTaggerShaderDef()
.Param("filter_linear", "true")
.Param("scale", "1.000000")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(DownsampleShadersMixedResSupportFxaaPlusAlphaShaderDef()
.Param("alias", "HiresSource")
.Param("filter_linear", "true")
.Param("scale", "1.000000")
.Param("scale_type", "source")
.Param("srgb_framebuffer", "true"));
         	ShaderDefs.push_back(DownsampleShadersMixedResBlurGaussHShaderDef()
.Param("filter_linear", "true")
.Param("scale", "0.333333")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(DownsampleShadersMixedResBlurGaussVShaderDef()
.Param("alias", "BlurSource")
.Param("filter_linear", "true")
.Param("scale_type_x", "source")
.Param("scale_type_y", "source")
.Param("scale_x", "1.0")
.Param("scale_y", "1.0"));
         	ShaderDefs.push_back(DenoisersShadersBilateralHorizontalShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(DenoisersShadersBilateralVerticalShaderDef()
.Param("alias", "XbrSource")
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(AntiAliasingShadersReverseAaShaderDef()
.Param("scale", "2.000000")
.Param("scale_type", "source")
.Param("srgb_framebuffer", "true"));
         	ShaderDefs.push_back(InterpolationShadersBSpline4TapsShaderDef()
.Param("filter_linear", "true")
.Param("scale", "1.0")
.Param("scale_type", "viewport")
.Param("srgb_framebuffer", "true"));
         	ShaderDefs.push_back(DeblurShadersDeblurLumaShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.000000")
.Param("scale_type", "source")
.Param("srgb_framebuffer", "true"));
         	ShaderDefs.push_back(DownsampleShadersMixedResOutputShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.000000")
.Param("scale_type", "source"));
	}
};
}
