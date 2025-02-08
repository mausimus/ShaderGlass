/*
ShaderGlass preset presets-handheld-plus-color-mod / simpletex_lcd+gbc-color-4k imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/presets/handheld-plus-color-mod/simpletex_lcd+gbc-color-4k.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class PresetsHandheldPlusColorModSimpletex_lcdGbcColor4kPresetDef : public PresetDef
{
public:
	PresetsHandheldPlusColorModSimpletex_lcdGbcColor4kPresetDef() : PresetDef{}
	{
		Name = "simpletex_lcd+gbc-color-4k";
		Category = "presets-handheld-plus-color-mod";
	}

	virtual void Build() {
         	ShaderDefs.push_back(HandheldShadersSimpletex_lcdSimpletex_lcdGbcColor4kShaderDef()
.Param("filter_linear", "true")
.Param("float_framebuffer", "false")
.Param("mipmap_input", "false")
.Param("scale_type", "viewport")
.Param("srgb_framebuffer", "false")
.Param("wrap_mode", "clamp_to_border"));
            TextureDefs.push_back(HandheldShadersSimpletex_lcdPng4kTextured_paperTextureDef()
.Param("linear", "false")
.Param("name", "BACKGROUND"));
            OverrideParam("BACKGROUND_INTENSITY", (float)1.000000);
            OverrideParam("DARKEN_COLOUR", (float)0.050000);
            OverrideParam("DARKEN_GRID", (float)0.000000);
            OverrideParam("GRID_BIAS", (float)0.600000);
            OverrideParam("GRID_INTENSITY", (float)0.650000);
            OverrideParam("GRID_WIDTH", (float)0.650000);
	}
};
}
