/*
ShaderGlass preset downsample / mixed-res-4x-crt-hyllian imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/downsample/mixed-res-4x-crt-hyllian.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class DownsampleMixedRes4xCrtHyllianPresetDef : public PresetDef
{
public:
	DownsampleMixedRes4xCrtHyllianPresetDef() : PresetDef{}
	{
		Name = "mixed-res-4x-crt-hyllian";
		Category = "downsample";
	}

	virtual void Build() {
         	ShaderDefs.push_back(DownsampleShadersMixedResMultiLUTModifiedShaderDef()
.Param("scale", "1.000000")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(DownsampleShadersMixedResCoderShaderDef()
.Param("scale", "1.000000")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(DownsampleShadersMixedResHiresTaggerShaderDef()
.Param("alias", "HiresSource")
.Param("filter_linear", "true")
.Param("scale", "1.000000")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(DownsampleShadersMixedResBlurGaussHShaderDef()
.Param("filter_linear", "true")
.Param("scale", "0.25")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(DownsampleShadersMixedResBlurGaussVShaderDef()
.Param("alias", "BlurSource")
.Param("filter_linear", "true")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(DenoisersShadersBilateralHorizontalShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(DenoisersShadersBilateralVerticalShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(CrtShadersHyllianCrtHyllianPass0ShaderDef()
.Param("filter_linear", "false")
.Param("scale_type_x", "viewport")
.Param("scale_type_y", "source")
.Param("scale_x", "1.000000")
.Param("scale_y", "1.000000")
.Param("srgb_framebuffer", "true"));
         	ShaderDefs.push_back(CrtShadersHyllianCrtHyllianPass1ShaderDef()
.Param("filter_linear", "false")
.Param("scale_type_x", "source")
.Param("scale_type_y", "viewport")
.Param("scale_x", "1.000000")
.Param("scale_y", "1.000000")
.Param("srgb_framebuffer", "true"));
         	ShaderDefs.push_back(DownsampleShadersMixedResOutputShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.000000")
.Param("scale_type", "source"));
            TextureDefs.push_back(ReshadeShadersLUTGradeCompositeTextureDef()
.Param("linear", "true")
.Param("name", "SamplerLUT1"));
            TextureDefs.push_back(ReshadeShadersLUTGradeRgbTextureDef()
.Param("linear", "true")
.Param("name", "SamplerLUT2"));
	}
};
}
