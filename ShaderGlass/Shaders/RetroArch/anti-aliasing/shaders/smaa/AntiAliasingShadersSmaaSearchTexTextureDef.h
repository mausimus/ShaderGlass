/*
ShaderGlass texture anti-aliasing-shaders-smaa / SearchTex imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/6f921ee4815a7894a33855974285b04545a4fa42/anti-aliasing/shaders/smaa/SearchTex.png
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace AntiAliasingShadersSmaaSearchTexTextureDefs
{
const BYTE sData[] =
{137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,0,0,0,64,0,0,0,16,2,3,0,0,0,219,137,145,186,0,0,0,9,80,76,84
,69,254,0,0,127,0,0,0,0,0,39,35,78,220,0,0,0,48,73,68,65,84,120,218,99,224,204,80,93,181,106,90,84,164,219,170,168,85,32,192
,196,128,14,86,161,1,6,50,180,96,8,68,102,166,130,205,8,195,109,6,229,90,200,16,0,0,39,183,117,135,253,183,51,200,0,0,0,0,73,69
,78,68,174,66,96,130};
}

class AntiAliasingShadersSmaaSearchTexTextureDef : public TextureDef
{
public:
	AntiAliasingShadersSmaaSearchTexTextureDef() : TextureDef{}
	{
		Name = "SearchTex.png";
		Data = AntiAliasingShadersSmaaSearchTexTextureDefs::sData;
		DataLength = sizeof(AntiAliasingShadersSmaaSearchTexTextureDefs::sData);
	}
};
