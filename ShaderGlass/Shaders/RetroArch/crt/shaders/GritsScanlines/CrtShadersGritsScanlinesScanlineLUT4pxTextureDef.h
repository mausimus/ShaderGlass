/*
ShaderGlass texture crt-shaders-GritsScanlines / Scanline-LUT-4px imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/crt/shaders/GritsScanlines/Scanline-LUT-4px.png
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace CrtShadersGritsScanlinesScanlineLUT4pxTextureDefs
{
const BYTE sData[] =
{137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,0,0,0,4,0,0,0,4,8,0,0,0,0,140,154,193,162,0,0,0,28,73,68,65
,84,120,1,99,204,52,210,215,102,252,255,143,129,129,113,158,138,130,32,163,134,182,185,37,0,53,248,4,173,17,7,225,5,0,0,0,0,73,69,78
,68,174,66,96,130};
}

class CrtShadersGritsScanlinesScanlineLUT4pxTextureDef : public TextureDef
{
public:
	CrtShadersGritsScanlinesScanlineLUT4pxTextureDef() : TextureDef{}
	{
		Name = "Scanline-LUT-4px.png";
		Data = CrtShadersGritsScanlinesScanlineLUT4pxTextureDefs::sData;
		DataLength = sizeof(CrtShadersGritsScanlinesScanlineLUT4pxTextureDefs::sData);
	}
};
