/*
ShaderGlass texture anti-aliasing-shaders-smaa / SearchTex imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/anti-aliasing/shaders/smaa/SearchTex.png
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace AntiAliasingShadersSmaaSearchTexTextureDefs
{
const BYTE sData[] =
{137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,0,0,0,64,0,0,0,16,2,3,0,0,0,219,137,145,186,0,0,0,9,80,76,84
,69,0,0,0,127,127,127,254,254,254,94,169,220,242,0,0,0,44,73,68,65,84,24,211,99,88,232,212,202,192,32,18,16,152,194,16,192,0,2,24
,2,152,128,176,22,194,32,208,209,21,172,37,132,33,0,67,128,124,45,148,3,0,28,34,20,5,10,205,8,234,0,0,0,0,73,69,78,68,174,66
,96,130};
}

class AntiAliasingShadersSmaaSearchTexTextureDef : public TextureDef
{
public:
	AntiAliasingShadersSmaaSearchTexTextureDef() : TextureDef{}
	{
		Data = AntiAliasingShadersSmaaSearchTexTextureDefs::sData;
		DataLength = sizeof(AntiAliasingShadersSmaaSearchTexTextureDefs::sData);
	}
};
