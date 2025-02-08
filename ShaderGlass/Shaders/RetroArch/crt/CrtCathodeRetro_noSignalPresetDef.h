/*
ShaderGlass preset crt / cathode-retro_no-signal imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/crt/cathode-retro_no-signal.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class CrtCathodeRetro_noSignalPresetDef : public PresetDef
{
public:
	CrtCathodeRetro_noSignalPresetDef() : PresetDef{}
	{
		Name = "cathode-retro_no-signal";
		Category = "crt";
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
         	ShaderDefs.push_back(StockStockShaderDef());
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
         	ShaderDefs.push_back(CrtShadersCathodeRetroCathodeRetroCrtGenerateMasksShaderDef()
.Param("alias", "g_maskTexture")
.Param("mipmap_input", "true")
.Param("scale_type", "viewport")
.Param("wrap_mode", "repeat"));
         	ShaderDefs.push_back(CrtShadersCathodeRetroCathodeRetroCrtGenerateScreenTextureShaderDef()
.Param("alias", "g_screenMaskTexture")
.Param("filter_linear", "true")
.Param("mipmap_input", "true")
.Param("wrap_mode", "clamp"));
         	ShaderDefs.push_back(CrtShadersCathodeRetroCathodeRetroCrtRgbToCrt_noSignalShaderDef()
.Param("filter_linear", "true")
.Param("mipmap_input", "true"));
	}
};
}
