/*
ShaderGlass texture crt-shaders-crt-potato-resources / crt-potato-thin imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/crt/shaders/crt-potato/resources/crt-potato-thin.png
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace CrtShadersCrtPotatoResourcesCrtPotatoThinTextureDefs
{
const BYTE sData[] =
{137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,0,0,0,2,0,0,0,5,8,2,0,0,0,224,209,170,203,0,0,0,46,73,68,65
,84,120,1,99,152,212,248,180,102,198,97,134,39,199,255,31,122,245,159,225,255,135,255,175,255,255,103,120,123,238,255,201,79,255,25,102,180,189,110,154
,119,2,0,166,143,24,97,79,78,61,146,0,0,0,0,73,69,78,68,174,66,96,130};
}

class CrtShadersCrtPotatoResourcesCrtPotatoThinTextureDef : public TextureDef
{
public:
	CrtShadersCrtPotatoResourcesCrtPotatoThinTextureDef() : TextureDef{}
	{
		Name = "crt-potato-thin.png";
		Data = CrtShadersCrtPotatoResourcesCrtPotatoThinTextureDefs::sData;
		DataLength = sizeof(CrtShadersCrtPotatoResourcesCrtPotatoThinTextureDefs::sData);
	}
};
