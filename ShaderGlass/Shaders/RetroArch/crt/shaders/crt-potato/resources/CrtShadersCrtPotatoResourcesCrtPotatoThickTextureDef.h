/*
ShaderGlass texture crt-shaders-crt-potato-resources / crt-potato-thick imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/crt/shaders/crt-potato/resources/crt-potato-thick.png
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace CrtShadersCrtPotatoResourcesCrtPotatoThickTextureDefs
{
const BYTE sData[] =
{137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,0,0,0,2,0,0,0,5,8,2,0,0,0,224,209,170,203,0,0,0,42,73,68,65
,84,120,1,99,88,248,127,214,172,255,11,25,126,252,127,255,254,255,119,134,255,96,192,240,163,241,227,199,166,31,12,11,39,205,154,53,121,33,0,194
,87,24,170,30,150,109,75,0,0,0,0,73,69,78,68,174,66,96,130};
}

class CrtShadersCrtPotatoResourcesCrtPotatoThickTextureDef : public TextureDef
{
public:
	CrtShadersCrtPotatoResourcesCrtPotatoThickTextureDef() : TextureDef{}
	{
		Name = "crt-potato-thick.png";
		Data = CrtShadersCrtPotatoResourcesCrtPotatoThickTextureDefs::sData;
		DataLength = sizeof(CrtShadersCrtPotatoResourcesCrtPotatoThickTextureDefs::sData);
	}
};
