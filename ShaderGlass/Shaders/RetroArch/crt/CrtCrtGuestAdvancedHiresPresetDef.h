/*
ShaderGlass preset crt / crt-guest-advanced-hires imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/crt/crt-guest-advanced-hires.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class CrtCrtGuestAdvancedHiresPresetDef : public PresetDef
{
public:
	CrtCrtGuestAdvancedHiresPresetDef() : PresetDef{}
	{
		Name = "crt-guest-advanced-hires";
		Category = "crt";
	}

	virtual void Build() {
         	ShaderDefs.push_back(StockStockShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(StockStockShaderDef()
.Param("alias", "StockPass")
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(CrtShadersGuestAdvancedAfterglow0ShaderDef()
.Param("alias", "AfterglowPass")
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(CrtShadersGuestAdvancedPreShadersAfterglowShaderDef()
.Param("alias", "PrePass")
.Param("filter_linear", "false")
.Param("mipmap_input", "true")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(CrtShadersGuestAdvancedAvgLumHiresShaderDef()
.Param("alias", "AvgLumPass")
.Param("filter_linear", "true")
.Param("mipmap_input", "true")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(CrtShadersGuestAdvancedLinearizeHiresShaderDef()
.Param("alias", "LinearizePass")
.Param("filter_linear", "true")
.Param("float_framebuffer", "true")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(CrtShadersGuestAdvancedBloom_horizontal_ntscShaderDef()
.Param("filter_linear", "true")
.Param("float_framebuffer", "true")
.Param("scale_type_x", "absolute")
.Param("scale_type_y", "source")
.Param("scale_x", "800.0")
.Param("scale_y", "1.0"));
         	ShaderDefs.push_back(CrtShadersGuestAdvancedBloom_vertical_ntscShaderDef()
.Param("alias", "GlowPass")
.Param("filter_linear", "true")
.Param("float_framebuffer", "true")
.Param("scale_type_x", "absolute")
.Param("scale_type_y", "absolute")
.Param("scale_x", "800.0")
.Param("scale_y", "600.0"));
         	ShaderDefs.push_back(CrtShadersGuestAdvancedCrtGuestAdvancedHiresShaderDef()
.Param("filter_linear", "true")
.Param("scale_type", "viewport")
.Param("scale_x", "1.0")
.Param("scale_y", "1.0"));
         	ShaderDefs.push_back(CrtShadersGuestAdvancedDeconvergenceShaderDef()
.Param("filter_linear", "true")
.Param("scale_type", "viewport")
.Param("scale_x", "1.0")
.Param("scale_y", "1.0"));
            TextureDefs.push_back(CrtShadersGuestAdvancedLutTrinitronLutTextureDef()
.Param("linear", "true")
.Param("name", "SamplerLUT1"));
            TextureDefs.push_back(CrtShadersGuestAdvancedLutInvTrinitronLutTextureDef()
.Param("linear", "true")
.Param("name", "SamplerLUT2"));
            TextureDefs.push_back(CrtShadersGuestAdvancedLutNecLutTextureDef()
.Param("linear", "true")
.Param("name", "SamplerLUT3"));
            TextureDefs.push_back(CrtShadersGuestAdvancedLutNtscLutTextureDef()
.Param("linear", "true")
.Param("name", "SamplerLUT4"));
	}
};
}
