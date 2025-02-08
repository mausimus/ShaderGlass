/*
ShaderGlass preset presets-handheld-plus-color-mod / simpletex_lcd_720p+gbc-color imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/presets/handheld-plus-color-mod/simpletex_lcd_720p+gbc-color.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class PresetsHandheldPlusColorModSimpletex_lcd_720pGbcColorPresetDef : public PresetDef
{
public:
	PresetsHandheldPlusColorModSimpletex_lcd_720pGbcColorPresetDef() : PresetDef{}
	{
		Name = "simpletex_lcd_720p+gbc-color";
		Category = "presets-handheld-plus-color-mod";
	}

	virtual void Build() {
         	ShaderDefs.push_back(HandheldShadersSimpletex_lcdSimpletex_lcd_720pGbcColorShaderDef()
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
