/*
ShaderGlass preset handheld / simpletex_lcd+gba-color-4k imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/handheld/simpletex_lcd+gba-color-4k.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class HandheldSimpletex_lcdGbaColor4kPresetDef : public PresetDef
{
public:
	HandheldSimpletex_lcdGbaColor4kPresetDef() : PresetDef{}
	{
		Name = "simpletex_lcd+gba-color-4k";
		Category = "handheld";
	}

	virtual void Build() {
         	ShaderDefs.push_back(HandheldShadersSimpletex_lcdSimpletex_lcdGbaColor4kShaderDef()
.Param("filter_linear", "true")
.Param("float_framebuffer", "false")
.Param("mipmap_input", "false")
.Param("srgb_framebuffer", "false"));
            TextureDefs.push_back(HandheldShadersSimpletex_lcdPng4kTextured_paperTextureDef()
.Param("linear", "false")
.Param("name", "BACKGROUND"));
	}
};
}
