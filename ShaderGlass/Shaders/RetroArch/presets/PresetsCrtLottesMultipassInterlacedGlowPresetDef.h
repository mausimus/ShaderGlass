/*
ShaderGlass preset presets / crt-lottes-multipass-interlaced-glow imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/presets/crt-lottes-multipass-interlaced-glow.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class PresetsCrtLottesMultipassInterlacedGlowPresetDef : public PresetDef
{
public:
	PresetsCrtLottesMultipassInterlacedGlowPresetDef() : PresetDef{}
	{
		Name = "crt-lottes-multipass-interlaced-glow";
		Category = "presets";
	}

	virtual void Build() {
         	ShaderDefs.push_back(StockStockShaderDef()
.Param("alias", "glow_trails_refpass"));
         	ShaderDefs.push_back(CrtShadersGlowTrailsGlowTrails0ShaderDef()
.Param("alias", "Trails0")
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(BlursShadersRoyaleBlur9fastVerticalShaderDef()
.Param("filter_linear", "true")
.Param("scale", "1.0")
.Param("scale_type", "source")
.Param("srgb_framebuffer", "true"));
         	ShaderDefs.push_back(BlursShadersRoyaleBlur9fastHorizontalShaderDef()
.Param("alias", "TRAIL_BLUR")
.Param("filter_linear", "true")
.Param("scale", "1.0")
.Param("scale_type", "source")
.Param("srgb_framebuffer", "true"));
         	ShaderDefs.push_back(CrtShadersGlowTrailsGlowTrails1ShaderDef());
         	ShaderDefs.push_back(CrtShadersCrtRoyaleSrcCrtRoyaleFirstPassLinearizeCrtGammaBobFieldsShaderDef()
.Param("alias", "ORIG_LINEARIZED")
.Param("srgb_framebuffer", "true"));
         	ShaderDefs.push_back(CrtShadersCrtEasymodeHalationBlur_horizShaderDef()
.Param("filter_linear", "true")
.Param("scale", "1.0")
.Param("scale_type", "source")
.Param("srgb_framebuffer", "true"));
         	ShaderDefs.push_back(CrtShadersCrtEasymodeHalationBlur_vertShaderDef()
.Param("alias", "GlowPass")
.Param("filter_linear", "true")
.Param("scale", "1.0")
.Param("scale_type", "source")
.Param("srgb_framebuffer", "true"));
         	ShaderDefs.push_back(CrtShadersCrtEasymodeHalationThresholdShaderDef()
.Param("srgb_framebuffer", "true"));
         	ShaderDefs.push_back(CrtShadersCrtLottesMultipassBloompassGlowShaderDef()
.Param("alias", "BloomPass")
.Param("filter_linear", "true"));
         	ShaderDefs.push_back(CrtShadersCrtLottesMultipassScanpassGlowShaderDef()
.Param("filter_linear", "true"));
	}
};
}
