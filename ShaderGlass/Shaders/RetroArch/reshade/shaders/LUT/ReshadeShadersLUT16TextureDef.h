/*
ShaderGlass texture reshade-shaders-LUT / 16 imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/reshade/shaders/LUT/16.png
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace ReshadeShadersLUT16TextureDefs
{
const BYTE sData[] =
{137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,0,0,1,0,0,0,0,16,8,2,0,0,0,118,177,54,30,0,0,0,69,73,68,65
,84,120,1,237,211,177,21,0,16,24,131,193,192,2,255,254,203,154,65,7,119,47,101,218,111,38,169,157,181,148,255,51,127,255,153,202,183,64,0,8
,0,4,0,2,0,1,128,0,64,0,32,0,16,0,8,0,4,0,2,0,1,128,0,224,102,11,208,41,18,62,83,39,163,241,0,0,0,0,73,69
,78,68,174,66,96,130};
}

class ReshadeShadersLUT16TextureDef : public TextureDef
{
public:
	ReshadeShadersLUT16TextureDef() : TextureDef{}
	{
		Name = "16.png";
		Data = ReshadeShadersLUT16TextureDefs::sData;
		DataLength = sizeof(ReshadeShadersLUT16TextureDefs::sData);
	}
};
