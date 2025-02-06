/*
ShaderGlass preset crt-shaders-cathode-retro / signal_test imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/crt/shaders/cathode-retro/signal_test.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class CrtShadersCathodeRetroSignal_testPresetDef : public PresetDef
{
public:
	CrtShadersCathodeRetroSignal_testPresetDef() : PresetDef{}
	{
		Name = "signal_test";
		Category = "crt-shaders-cathode-retro";
	}

	virtual void Build() {
         	ShaderDefs.push_back(CrtShadersCathodeRetroCathodeRetroUtilCopyShaderDef()
.Param("alias", "g_sourceTexture")
.Param("filter_linear", "true"));
         	ShaderDefs.push_back(CrtShadersCathodeRetroCathodeRetroUtilDownsample2xHorzShaderDef()
.Param("filter_linear", "true")
.Param("scale_x", "0.5"));
         	ShaderDefs.push_back(CrtShadersCathodeRetroCathodeRetroUtilDownsample2xVertShaderDef()
.Param("filter_linear", "true")
.Param("scale_y", "0.5"));
         	ShaderDefs.push_back(CrtShadersCathodeRetroCathodeRetroUtilTonemapAndDownsampleHorzShaderDef()
.Param("filter_linear", "true")
.Param("scale_x", "0.5"));
         	ShaderDefs.push_back(CrtShadersCathodeRetroCathodeRetroUtilTonemapAndDownsampleVertShaderDef()
.Param("alias", "g_sourceTex")
.Param("filter_linear", "true")
.Param("scale_y", "0.5"));
         	ShaderDefs.push_back(CrtShadersCathodeRetroCathodeRetroUtilGaussianBlurHorzShaderDef()
.Param("filter_linear", "true"));
         	ShaderDefs.push_back(CrtShadersCathodeRetroCathodeRetroUtilGaussianBlurVertShaderDef()
.Param("alias", "g_diffusionTexture")
.Param("filter_linear", "true"));
         	ShaderDefs.push_back(CrtShadersCathodeRetroCathodeRetroGeneratorGenPhaseShaderDef()
.Param("alias", "g_scanlinePhases")
.Param("filter_linear", "true")
.Param("float_framebuffer", "true")
.Param("scale_type", "absolute")
.Param("scale_x", "224.0")
.Param("scale_y", "1.0"));
         	ShaderDefs.push_back(StockStockShaderDef()
.Param("scale_type_y", "absolute")
.Param("scale_y", "224.0"));
         	ShaderDefs.push_back(CrtShadersCathodeRetroCathodeRetroGeneratorRgbToSvideoOrCompositeShaderDef()
.Param("alias", "rgb2yiq")
.Param("float_framebuffer", "true"));
         	ShaderDefs.push_back(CrtShadersCathodeRetroCathodeRetroGeneratorApplyArtifactsShaderDef()
.Param("alias", "YIQ_tex")
.Param("filter_linear", "true")
.Param("float_framebuffer", "true"));
         	ShaderDefs.push_back(CrtShadersCathodeRetroCathodeRetroDecoderCompositeToSvideoShaderDef()
.Param("alias", "IQ_separate")
.Param("filter_linear", "true")
.Param("float_framebuffer", "true"));
         	ShaderDefs.push_back(CrtShadersCathodeRetroCathodeRetroDecoderSvideoToModulatedChromaShaderDef()
.Param("alias", "g_modulatedChromaTexture")
.Param("float_framebuffer", "true"));
         	ShaderDefs.push_back(CrtShadersCathodeRetroCathodeRetroDecoderSvideoToRgbShaderDef()
.Param("alias", "yiq2rgb")
.Param("float_framebuffer", "true"));
         	ShaderDefs.push_back(CrtShadersCathodeRetroCathodeRetroDecoderFilterRgbShaderDef()
.Param("alias", "g_currentFrameTexture")
.Param("float_framebuffer", "true"));
         	ShaderDefs.push_back(CrtShadersCathodeRetroCathodeRetroCrtGenerateMasksShaderDef()
.Param("alias", "g_maskTexture")
.Param("float_framebuffer", "true")
.Param("scale_type", "viewport"));
         	ShaderDefs.push_back(CrtShadersCathodeRetroCathodeRetroCrtGenerateScreenTextureShaderDef()
.Param("alias", "g_screenMaskTexture")
.Param("filter_linear", "true")
.Param("float_framebuffer", "true"));
         	ShaderDefs.push_back(CrtShadersCathodeRetroCathodeRetroCrtRgbToCrtShaderDef()
.Param("filter_linear", "true"));
	}
};
}
