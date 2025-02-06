/*
ShaderGlass preset presets-handheld-plus-color-mod / simpletex_lcd+gba-color imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/presets/handheld-plus-color-mod/simpletex_lcd+gba-color.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class PresetsHandheldPlusColorModSimpletex_lcdGbaColorPresetDef : public PresetDef
{
public:
	PresetsHandheldPlusColorModSimpletex_lcdGbaColorPresetDef() : PresetDef{}
	{
		Name = "simpletex_lcd+gba-color";
		Category = "presets-handheld-plus-color-mod";
	}

	virtual void Build() {
         	ShaderDefs.push_back(HandheldShadersSimpletex_lcdSimpletex_lcdGbaColorShaderDef()
.Param("filter_linear", "true")
.Param("float_framebuffer", "false")
.Param("mipmap_input", "false")
.Param("scale_type", "viewport")
.Param("srgb_framebuffer", "false"));
            TextureDefs.push_back(HandheldShadersSimpletex_lcdPng2kTextured_paperTextureDef()
.Param("linear", "false")
.Param("name", "BACKGROUND"));
	}
};
}
