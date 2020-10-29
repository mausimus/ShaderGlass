/*
ShaderGlass preset crt / crt-guest-sm imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/crt/crt-guest-sm.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class CrtCrtGuestSmPresetDef : public PresetDef
{
public:
	CrtCrtGuestSmPresetDef() : PresetDef{}
	{
		Name = "crt-guest-sm";
		Category = "crt";
	}

	virtual void Build() {
         	ShaderDefs.push_back(CrtShadersGuestCrtSmD65D50SmShaderDef()
.Param("alias", "WpPass")
.Param("filter_linear", "true")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(CrtShadersGuestCrtSmCrtGuestSmRot0ShaderDef()
.Param("alias", "RotPass")
.Param("filter_linear", "true")
.Param("float_framebuffer", "true")
.Param("scale_type_x", "source")
.Param("scale_type_y", "source")
.Param("scale_x", "1.0")
.Param("scale_y", "3.0"));
         	ShaderDefs.push_back(CrtShadersGuestCrtSmLinearizeSmShaderDef()
.Param("alias", "LinPass")
.Param("filter_linear", "true")
.Param("float_framebuffer", "true")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(CrtShadersGuestCrtSmBlur_horizSmShaderDef()
.Param("filter_linear", "true")
.Param("float_framebuffer", "true")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(CrtShadersGuestCrtSmBlur_vertSmShaderDef()
.Param("filter_linear", "true")
.Param("float_framebuffer", "true")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(CrtShadersGuestCrtSmCrtGuestSmShaderDef()
.Param("filter_linear", "true")
.Param("scale_type_x", "viewport")
.Param("scale_type_y", "viewport")
.Param("scale_x", "1.0")
.Param("scale_y", "1.0"));
	}
};
}
