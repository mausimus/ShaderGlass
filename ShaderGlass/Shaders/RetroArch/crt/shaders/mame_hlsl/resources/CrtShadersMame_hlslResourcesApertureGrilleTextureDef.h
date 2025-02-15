/*
ShaderGlass texture crt/shaders/mame_hlsl/resources / aperture-grille imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/crt/shaders/mame_hlsl/resources/aperture-grille.png
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace CrtShadersMame_hlslResourcesApertureGrilleTextureDefs
{
const BYTE sData[] =
{137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,0,0,0,48,0,0,0,48,2,3,0,0,0,42,108,17,20,0,0,0,9,80,76,84
,69,255,0,0,0,255,0,0,0,255,45,74,205,138,0,0,0,19,73,68,65,84,120,1,99,96,93,16,5,71,52,230,140,114,70,57,0,230,72,191
,65,103,187,241,240,0,0,0,0,73,69,78,68,174,66,96,130};
}

class CrtShadersMame_hlslResourcesApertureGrilleTextureDef : public TextureDef
{
public:
	CrtShadersMame_hlslResourcesApertureGrilleTextureDef() : TextureDef{}
	{
		Name = "aperture-grille.png";
		Data = CrtShadersMame_hlslResourcesApertureGrilleTextureDefs::sData;
		DataLength = sizeof(CrtShadersMame_hlslResourcesApertureGrilleTextureDefs::sData);
	}
};
