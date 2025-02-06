/*
ShaderGlass texture handheld-shaders-gb-palette-resources / palette-light imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/handheld/shaders/gb-palette/resources/palette-light.png
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace HandheldShadersGbPaletteResourcesPaletteLightTextureDefs
{
const BYTE sData[] =
{137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,0,0,0,64,0,0,1,0,2,3,0,0,0,126,40,152,149,0,0,0,12,80,76,84
,69,3,236,118,2,176,84,1,129,57,1,110,45,206,15,159,155,0,0,0,44,73,68,65,84,120,1,237,200,161,1,0,32,12,3,48,158,228,73,158
,4,95,179,13,157,200,44,128,95,59,8,33,68,55,78,16,66,136,110,220,32,132,16,205,120,97,203,248,106,46,131,216,121,0,0,0,0,73,69,78
,68,174,66,96,130};
}

class HandheldShadersGbPaletteResourcesPaletteLightTextureDef : public TextureDef
{
public:
	HandheldShadersGbPaletteResourcesPaletteLightTextureDef() : TextureDef{}
	{
		Name = "palette-light.png";
		Data = HandheldShadersGbPaletteResourcesPaletteLightTextureDefs::sData;
		DataLength = sizeof(HandheldShadersGbPaletteResourcesPaletteLightTextureDefs::sData);
	}
};
