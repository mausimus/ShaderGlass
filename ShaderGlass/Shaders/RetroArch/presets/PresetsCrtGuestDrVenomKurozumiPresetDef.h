/*
ShaderGlass preset presets / crt-guest-dr-venom-kurozumi imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/presets/crt-guest-dr-venom-kurozumi.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class PresetsCrtGuestDrVenomKurozumiPresetDef : public PresetDef
{
public:
	PresetsCrtGuestDrVenomKurozumiPresetDef() : PresetDef{}
	{
		Name = "crt-guest-dr-venom-kurozumi";
		Category = "presets";
	}

	virtual void Build() {
         	ShaderDefs.push_back(CrtShadersGuestLutLutShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.000000")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(CrtShadersGuestColorProfilesShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.000000")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(CrtShadersGuestD65D50ShaderDef()
.Param("alias", "WhitePointPass")
.Param("filter_linear", "false")
.Param("scale", "1.000000")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(CrtShadersGuestAfterglowShaderDef()
.Param("alias", "AfterglowPass")
.Param("filter_linear", "false")
.Param("scale", "1.000000")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(CrtShadersGuestAvgLum0ShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.000000")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(CrtShadersGuestAvgLumShaderDef()
.Param("alias", "AvgLumPass")
.Param("filter_linear", "false")
.Param("mipmap_input", "true")
.Param("scale", "1.000000")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(CrtShadersGuestLinearizeShaderDef()
.Param("alias", "LinearizePass")
.Param("filter_linear", "false")
.Param("float_framebuffer", "true")
.Param("scale", "1.000000")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(CrtShadersGuestBlur_horizShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.000000")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(CrtShadersGuestBlur_vertShaderDef()
.Param("alias", "GlowPass")
.Param("filter_linear", "false")
.Param("scale", "1.000000")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(CrtShadersGuestLinearize_scanlinesShaderDef()
.Param("filter_linear", "true")
.Param("float_framebuffer", "true")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(CrtShadersGuestCrtGuestDrVenomShaderDef()
.Param("filter_linear", "true")
.Param("scale_type", "viewport")
.Param("scale_x", "1.0")
.Param("scale_y", "1.0"));
            TextureDefs.push_back(CrtShadersGuestLutSony_trinitron1TextureDef()
.Param("linear", "true")
.Param("mipmap", "false")
.Param("name", "SamplerLUT1")
.Param("wrap_mode", "clamp_to_border"));
            TextureDefs.push_back(CrtShadersGuestLutSony_trinitron2TextureDef()
.Param("linear", "true")
.Param("mipmap", "false")
.Param("name", "SamplerLUT2")
.Param("wrap_mode", "clamp_to_border"));
            TextureDefs.push_back(CrtShadersGuestLutOther1TextureDef()
.Param("linear", "true")
.Param("mipmap", "false")
.Param("name", "SamplerLUT3")
.Param("wrap_mode", "clamp_to_border"));
	}
};
}
