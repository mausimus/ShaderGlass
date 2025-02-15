/*
ShaderGlass texture crt/shaders/geom-deluxe/masks / delta_2_4x1_rgb imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/crt/shaders/geom-deluxe/masks/delta_2_4x1_rgb.png
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace CrtShadersGeomDeluxeMasksDelta_2_4x1_rgbTextureDefs
{
const BYTE sData[] =
{137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,0,0,0,4,0,0,0,2,8,6,0,0,0,127,168,125,99,0,0,0,28,73,68,65
,84,120,1,99,252,207,192,240,155,1,72,48,130,9,48,253,255,55,136,241,31,66,48,0,0,166,64,8,246,215,207,154,119,0,0,0,0,73,69,78
,68,174,66,96,130};
}

class CrtShadersGeomDeluxeMasksDelta_2_4x1_rgbTextureDef : public TextureDef
{
public:
	CrtShadersGeomDeluxeMasksDelta_2_4x1_rgbTextureDef() : TextureDef{}
	{
		Name = "delta_2_4x1_rgb.png";
		Data = CrtShadersGeomDeluxeMasksDelta_2_4x1_rgbTextureDefs::sData;
		DataLength = sizeof(CrtShadersGeomDeluxeMasksDelta_2_4x1_rgbTextureDefs::sData);
	}
};
