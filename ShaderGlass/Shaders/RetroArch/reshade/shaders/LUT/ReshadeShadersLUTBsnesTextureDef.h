/*
ShaderGlass texture reshade-shaders-LUT / bsnes imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/reshade/shaders/LUT/bsnes.png
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace ReshadeShadersLUTBsnesTextureDefs
{
const BYTE sData[] =
{137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,0,0,4,0,0,0,0,32,8,2,0,0,0,3,156,157,241,0,0,1,30,73,68,65
,84,120,1,236,211,199,17,194,64,20,198,96,109,78,94,123,251,175,150,18,192,192,79,124,51,58,127,55,69,192,129,135,0,17,18,100,40,80,161,65
,135,1,27,76,216,225,128,5,245,230,10,56,156,216,247,98,63,136,253,40,246,147,216,207,98,191,136,253,42,246,155,216,239,98,127,136,253,77,236,79
,177,191,139,253,67,236,175,143,252,203,124,243,205,55,223,252,34,245,35,14,46,237,215,139,81,66,1,0,3,176,158,246,125,16,80,247,159,150,65,154
,108,17,54,0,0,64,243,149,17,0,0,64,243,157,17,0,0,64,211,0,0,0,35,154,35,0,0,192,136,230,12,0,0,48,162,185,2,0,0,140
,104,238,0,35,0,0,154,71,70,0,0,0,205,79,70,0,0,0,205,51,35,0,0,128,230,149,17,0,0,64,243,206,8,0,0,160,249,205,8,0
,0,160,249,203,8,0,0,160,249,15,48,2,0,160,185,51,96,0,0,0,203,4,0,0,0,16,0,0,0,16,0,0,0,64,0,0,0,0,1,0
,0,0,4,0,0,0,16,0,0,0,64,0,0,0,0,1,0,0,0,4,0,0,0,16,0,0,0,64,0,0,0,0,1,0,0,0,154,43,35,0
,0,128,15,150,185,34,126,13,17,189,81,0,0,0,0,73,69,78,68,174,66,96,130};
}

class ReshadeShadersLUTBsnesTextureDef : public TextureDef
{
public:
	ReshadeShadersLUTBsnesTextureDef() : TextureDef{}
	{
		Name = "bsnes.png";
		Data = ReshadeShadersLUTBsnesTextureDefs::sData;
		DataLength = sizeof(ReshadeShadersLUTBsnesTextureDefs::sData);
	}
};
