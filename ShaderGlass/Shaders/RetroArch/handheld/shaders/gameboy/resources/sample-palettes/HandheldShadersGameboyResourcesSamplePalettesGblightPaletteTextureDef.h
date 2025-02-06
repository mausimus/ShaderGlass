/*
ShaderGlass texture handheld-shaders-gameboy-resources-sample-palettes / gblight-palette imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/handheld/shaders/gameboy/resources/sample-palettes/gblight-palette.png
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace HandheldShadersGameboyResourcesSamplePalettesGblightPaletteTextureDefs
{
const BYTE sData[] =
{137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,0,0,0,128,0,0,0,64,1,3,0,0,0,232,24,237,60,0,0,0,6,80,76,84
,69,5,186,95,0,83,52,140,145,221,31,0,0,0,23,73,68,65,84,120,1,99,128,129,255,80,64,164,192,168,192,168,192,168,192,168,0,0,11,76
,254,16,194,179,25,208,0,0,0,0,73,69,78,68,174,66,96,130};
}

class HandheldShadersGameboyResourcesSamplePalettesGblightPaletteTextureDef : public TextureDef
{
public:
	HandheldShadersGameboyResourcesSamplePalettesGblightPaletteTextureDef() : TextureDef{}
	{
		Name = "gblight-palette.png";
		Data = HandheldShadersGameboyResourcesSamplePalettesGblightPaletteTextureDefs::sData;
		DataLength = sizeof(HandheldShadersGameboyResourcesSamplePalettesGblightPaletteTextureDefs::sData);
	}
};
