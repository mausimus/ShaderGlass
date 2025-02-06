/*
ShaderGlass preset crt / crt-guest-advanced-hd imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/crt/crt-guest-advanced-hd.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class CrtCrtGuestAdvancedHdPresetDef : public PresetDef
{
public:
	CrtCrtGuestAdvancedHdPresetDef() : PresetDef{}
	{
		Name = "crt-guest-advanced-hd";
		Category = "crt";
	}

	virtual void Build() {
         	ShaderDefs.push_back(CrtShadersGuestHdStockShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(CrtShadersGuestHdStockShaderDef()
.Param("alias", "StockPass")
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(CrtShadersGuestHdAfterglow0ShaderDef()
.Param("alias", "AfterglowPass")
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(CrtShadersGuestHdPreShadersAfterglowShaderDef()
.Param("alias", "PrePass")
.Param("filter_linear", "false")
.Param("mipmap_input", "true")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(CrtShadersGuestHdLinearizeHdShaderDef()
.Param("alias", "LinearizePass")
.Param("filter_linear", "true")
.Param("float_framebuffer", "true")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(CrtShadersGuestHdCrtGuestAdvancedHdPass1ShaderDef()
.Param("alias", "Pass1")
.Param("filter_linear", "true")
.Param("float_framebuffer", "true")
.Param("scale_type_x", "viewport")
.Param("scale_type_y", "source")
.Param("scale_x", "1.0")
.Param("scale_y", "1.0"));
         	ShaderDefs.push_back(CrtShadersGuestHdGaussian_horizontalShaderDef()
.Param("filter_linear", "true")
.Param("float_framebuffer", "true")
.Param("scale_type_x", "absolute")
.Param("scale_type_y", "source")
.Param("scale_x", "800.0")
.Param("scale_y", "1.0"));
         	ShaderDefs.push_back(CrtShadersGuestHdGaussian_verticalShaderDef()
.Param("alias", "GlowPass")
.Param("filter_linear", "true")
.Param("float_framebuffer", "true")
.Param("scale_type_x", "absolute")
.Param("scale_type_y", "absolute")
.Param("scale_x", "800.0")
.Param("scale_y", "600.0"));
         	ShaderDefs.push_back(CrtShadersGuestHdBloom_horizontalShaderDef()
.Param("filter_linear", "true")
.Param("float_framebuffer", "true")
.Param("scale_type_x", "absolute")
.Param("scale_type_y", "source")
.Param("scale_x", "800.0")
.Param("scale_y", "1.0"));
         	ShaderDefs.push_back(CrtShadersGuestHdBloom_verticalShaderDef()
.Param("alias", "BloomPass")
.Param("filter_linear", "true")
.Param("float_framebuffer", "true")
.Param("scale_type_x", "source")
.Param("scale_type_y", "absolute")
.Param("scale_x", "1.0")
.Param("scale_y", "600.0"));
         	ShaderDefs.push_back(CrtShadersGuestHdCrtGuestAdvancedHdPass2ShaderDef()
.Param("filter_linear", "true")
.Param("float_framebuffer", "true")
.Param("scale_type", "viewport")
.Param("scale_x", "1.0")
.Param("scale_y", "1.0"));
         	ShaderDefs.push_back(CrtShadersGuestHdDeconvergenceHdShaderDef()
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
