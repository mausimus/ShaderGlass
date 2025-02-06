/*
ShaderGlass texture crt-shaders-phosphorlut-luts / slotmask imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/crt/shaders/phosphorlut/luts/slotmask.png
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace CrtShadersPhosphorlutLutsSlotmaskTextureDefs
{
const BYTE sData[] =
{137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,0,0,0,20,0,0,0,20,8,2,0,0,0,2,235,138,90,0,0,0,38,73,68,65
,84,120,1,99,32,8,254,67,17,16,243,64,145,196,127,8,34,32,61,34,53,227,5,35,49,192,70,83,216,104,10,163,173,102,0,39,81,81,252,114
,249,174,192,0,0,0,0,73,69,78,68,174,66,96,130};
}

class CrtShadersPhosphorlutLutsSlotmaskTextureDef : public TextureDef
{
public:
	CrtShadersPhosphorlutLutsSlotmaskTextureDef() : TextureDef{}
	{
		Name = "slotmask.png";
		Data = CrtShadersPhosphorlutLutsSlotmaskTextureDefs::sData;
		DataLength = sizeof(CrtShadersPhosphorlutLutsSlotmaskTextureDefs::sData);
	}
};
