/*
ShaderGlass preset handheld / simpletex_lcd_720p imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/handheld/simpletex_lcd_720p.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class HandheldSimpletex_lcd_720pPresetDef : public PresetDef
{
public:
	HandheldSimpletex_lcd_720pPresetDef() : PresetDef{}
	{
		Name = "simpletex_lcd_720p";
		Category = "handheld";
	}

	virtual void Build() {
         	ShaderDefs.push_back(HandheldShadersSimpletex_lcdSimpletex_lcd_720pShaderDef()
.Param("filter_linear", "true")
.Param("float_framebuffer", "false")
.Param("mipmap_input", "false")
.Param("scale_type", "viewport")
.Param("srgb_framebuffer", "false")
.Param("wrap_mode", "clamp_to_border"));
            TextureDefs.push_back(HandheldShadersSimpletex_lcdPng2kTextured_paperTextureDef()
.Param("linear", "false")
.Param("name", "BACKGROUND"));
            OverrideParam("BACKGROUND_INTENSITY", (float)1.000000);
            OverrideParam("DARKEN_COLOUR", (float)0.100000);
            OverrideParam("DARKEN_GRID", (float)0.000000);
            OverrideParam("GRID_BIAS", (float)0.800000);
            OverrideParam("GRID_INTENSITY", (float)0.720000);
            OverrideParam("GRID_WIDTH", (float)0.940000);
	}
};
}
