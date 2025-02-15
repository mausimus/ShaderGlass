/*
ShaderGlass texture crt/shaders/geom-deluxe/masks / aperture_2_4_rgb imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/crt/shaders/geom-deluxe/masks/aperture_2_4_rgb.png
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace CrtShadersGeomDeluxeMasksAperture_2_4_rgbTextureDefs
{
const BYTE sData[] =
{137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,0,0,0,4,0,0,0,1,8,6,0,0,0,249,60,15,205,0,0,0,19,73,68,65
,84,120,1,99,252,207,192,240,151,1,72,48,66,8,6,0,45,229,3,254,96,62,58,45,0,0,0,0,73,69,78,68,174,66,96,130};
}

class CrtShadersGeomDeluxeMasksAperture_2_4_rgbTextureDef : public TextureDef
{
public:
	CrtShadersGeomDeluxeMasksAperture_2_4_rgbTextureDef() : TextureDef{}
	{
		Name = "aperture_2_4_rgb.png";
		Data = CrtShadersGeomDeluxeMasksAperture_2_4_rgbTextureDefs::sData;
		DataLength = sizeof(CrtShadersGeomDeluxeMasksAperture_2_4_rgbTextureDefs::sData);
	}
};
