/*
ShaderGlass texture crt/shaders/phosphorlut/luts / shadowmask imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/crt/shaders/phosphorlut/luts/shadowmask.png
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace CrtShadersPhosphorlutLutsShadowmaskTextureDefs
{
const BYTE sData[] =
{137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,0,0,0,18,0,0,0,18,8,2,0,0,0,217,172,25,0,0,0,0,52,73,68,65
,84,120,1,99,144,251,207,0,4,64,2,68,35,152,64,113,60,82,8,30,66,26,166,7,161,19,67,21,185,218,200,116,36,78,35,113,59,132,2,109
,131,45,36,71,67,114,52,36,1,124,134,223,153,41,133,74,161,0,0,0,0,73,69,78,68,174,66,96,130};
}

class CrtShadersPhosphorlutLutsShadowmaskTextureDef : public TextureDef
{
public:
	CrtShadersPhosphorlutLutsShadowmaskTextureDef() : TextureDef{}
	{
		Name = "shadowmask.png";
		Data = CrtShadersPhosphorlutLutsShadowmaskTextureDefs::sData;
		DataLength = sizeof(CrtShadersPhosphorlutLutsShadowmaskTextureDefs::sData);
	}
};
