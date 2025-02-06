/*
ShaderGlass texture handheld-shaders-gameboy-resources-sample-palettes / dmg-palette-0 imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/handheld/shaders/gameboy/resources/sample-palettes/dmg-palette-0.png
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace HandheldShadersGameboyResourcesSamplePalettesDmgPalette0TextureDefs
{
const BYTE sData[] =
{137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,0,0,0,128,0,0,0,64,1,3,0,0,0,232,24,237,60,0,0,0,6,80,76,84
,69,160,170,5,29,106,107,224,190,212,250,0,0,0,23,73,68,65,84,120,1,99,128,129,255,80,64,164,192,168,192,168,192,168,192,168,0,0,11,76
,254,16,194,179,25,208,0,0,0,0,73,69,78,68,174,66,96,130};
}

class HandheldShadersGameboyResourcesSamplePalettesDmgPalette0TextureDef : public TextureDef
{
public:
	HandheldShadersGameboyResourcesSamplePalettesDmgPalette0TextureDef() : TextureDef{}
	{
		Name = "dmg-palette-0.png";
		Data = HandheldShadersGameboyResourcesSamplePalettesDmgPalette0TextureDefs::sData;
		DataLength = sizeof(HandheldShadersGameboyResourcesSamplePalettesDmgPalette0TextureDefs::sData);
	}
};
