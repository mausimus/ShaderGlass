/*
ShaderGlass texture handheld-shaders-gameboy-resources-sample-palettes / b-w-palette imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/handheld/shaders/gameboy/resources/sample-palettes/b-w-palette.png
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace HandheldShadersGameboyResourcesSamplePalettesBWPaletteTextureDefs
{
const BYTE sData[] =
{137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,0,0,0,128,0,0,0,64,1,0,0,0,0,250,173,66,210,0,0,0,24,73,68,65
,84,120,1,99,248,15,5,12,80,64,164,192,168,192,168,192,168,192,168,0,0,252,45,254,16,23,125,218,173,0,0,0,0,73,69,78,68,174,66,96
,130};
}

class HandheldShadersGameboyResourcesSamplePalettesBWPaletteTextureDef : public TextureDef
{
public:
	HandheldShadersGameboyResourcesSamplePalettesBWPaletteTextureDef() : TextureDef{}
	{
		Name = "b-w-palette.png";
		Data = HandheldShadersGameboyResourcesSamplePalettesBWPaletteTextureDefs::sData;
		DataLength = sizeof(HandheldShadersGameboyResourcesSamplePalettesBWPaletteTextureDefs::sData);
	}
};
