/*
ShaderGlass preset handheld / simpletex_lcd imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/handheld/simpletex_lcd.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class HandheldSimpletex_lcdPresetDef : public PresetDef
{
public:
	HandheldSimpletex_lcdPresetDef() : PresetDef{}
	{
		Name = "simpletex_lcd";
		Category = "handheld";
	}

	virtual void Build() {
         	ShaderDefs.push_back(HandheldShadersSimpletex_lcdSimpletex_lcdShaderDef()
.Param("filter_linear", "true")
.Param("float_framebuffer", "false")
.Param("mipmap_input", "false")
.Param("srgb_framebuffer", "false"));
            TextureDefs.push_back(HandheldShadersSimpletex_lcdPng2kTextured_paperTextureDef()
.Param("linear", "false")
.Param("name", "BACKGROUND"));
	}
};
}
