/*
ShaderGlass preset downsample-mixed-res-hooks / mixed-res-4x-prepend imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/downsample/mixed-res/hooks/mixed-res-4x-prepend.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class DownsampleMixedResHooksMixedRes4xPrependPresetDef : public PresetDef
{
public:
	DownsampleMixedResHooksMixedRes4xPrependPresetDef() : PresetDef{}
	{
		Name = "mixed-res-4x-prepend";
		Category = "downsample-mixed-res-hooks";
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
            TextureDefs.push_back(ReshadeShadersLUTGradeCompositeTextureDef()
.Param("linear", "true")
.Param("name", "SamplerLUT1"));
            TextureDefs.push_back(ReshadeShadersLUTGradeRgbTextureDef()
.Param("linear", "true")
.Param("name", "SamplerLUT2"));
	}
};
}
