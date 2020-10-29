/*
ShaderGlass preset crt / crt-guest-dr-venom-fast imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/crt/crt-guest-dr-venom-fast.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class CrtCrtGuestDrVenomFastPresetDef : public PresetDef
{
public:
	CrtCrtGuestDrVenomFastPresetDef() : PresetDef{}
	{
		Name = "crt-guest-dr-venom-fast";
		Category = "crt";
	}

	virtual void Build() {
         	ShaderDefs.push_back(CrtShadersGuestLutLutShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(CrtShadersGuestFastSmoothingShaderDef()
.Param("alias", "SmoothPass")
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(CrtShadersGuestFastLinearizeMultipassShaderDef()
.Param("filter_linear", "false")
.Param("float_framebuffer", "true")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(CrtShadersGuestFastCrtGuestDrVenomPass1ShaderDef()
.Param("filter_linear", "false")
.Param("float_framebuffer", "true")
.Param("scale_type_x", "viewport")
.Param("scale_type_y", "source")
.Param("scale_x", "1.0")
.Param("scale_y", "1.0"));
         	ShaderDefs.push_back(CrtShadersGuestFastCrtGuestDrVenomPass2ShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "viewport")
.Param("scale_x", "1.0")
.Param("scale_y", "1.0"));
            TextureDefs.push_back(CrtShadersGuestLutSony_trinitron1TextureDef()
.Param("linear", "true")
.Param("name", "SamplerLUT1"));
            TextureDefs.push_back(CrtShadersGuestLutSony_trinitron2TextureDef()
.Param("linear", "true")
.Param("name", "SamplerLUT2"));
            TextureDefs.push_back(CrtShadersGuestLutOther1TextureDef()
.Param("linear", "true")
.Param("name", "SamplerLUT3"));
	}
};
}
