/*
ShaderGlass preset crt / crt-lottes-multipass-glow imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/crt/crt-lottes-multipass-glow.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class CrtCrtLottesMultipassGlowPresetDef : public PresetDef
{
public:
	CrtCrtLottesMultipassGlowPresetDef() : PresetDef{}
	{
		Name = "crt-lottes-multipass-glow";
		Category = "crt";
	}

	virtual void Build() {
         	ShaderDefs.push_back(CrtShadersCrtEasymodeHalationLinearizeShaderDef()
.Param("alias", "ORIG_LINEARIZED")
.Param("filter_linear", "true")
.Param("scale_type_x", "source")
.Param("scale_type_y", "source")
.Param("scale_x", "1.000000")
.Param("scale_y", "1.000000")
.Param("srgb_framebuffer", "true"));
         	ShaderDefs.push_back(CrtShadersCrtEasymodeHalationBlur_horizShaderDef()
.Param("filter_linear", "false")
.Param("scale_type_x", "source")
.Param("scale_type_y", "source")
.Param("scale_x", "1.000000")
.Param("scale_y", "1.000000")
.Param("srgb_framebuffer", "true"));
         	ShaderDefs.push_back(CrtShadersCrtEasymodeHalationBlur_vertShaderDef()
.Param("filter_linear", "false")
.Param("scale_type_x", "source")
.Param("scale_type_y", "source")
.Param("scale_x", "1.000000")
.Param("scale_y", "1.000000")
.Param("srgb_framebuffer", "true"));
         	ShaderDefs.push_back(CrtShadersCrtEasymodeHalationThresholdShaderDef()
.Param("alias", "GlowPass")
.Param("filter_linear", "false")
.Param("scale_type_x", "source")
.Param("scale_type_y", "source")
.Param("scale_x", "1.000000")
.Param("scale_y", "1.000000")
.Param("srgb_framebuffer", "true"));
         	ShaderDefs.push_back(CrtShadersCrtLottesMultipassBloompassShaderDef()
.Param("alias", "BloomPass")
.Param("filter_linear", "true")
.Param("srgb_framebuffer", "true"));
         	ShaderDefs.push_back(CrtShadersCrtLottesMultipassScanpassGlowShaderDef()
.Param("filter_linear", "true"));
	}
};
}
