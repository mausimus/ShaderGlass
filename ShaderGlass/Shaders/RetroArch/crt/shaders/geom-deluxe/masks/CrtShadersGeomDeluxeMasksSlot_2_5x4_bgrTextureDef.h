/*
ShaderGlass texture crt-shaders-geom-deluxe-masks / slot_2_5x4_bgr imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/6f921ee4815a7894a33855974285b04545a4fa42/crt/shaders/geom-deluxe/masks/slot_2_5x4_bgr.png
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace CrtShadersGeomDeluxeMasksSlot_2_5x4_bgrTextureDefs
{
const BYTE sData[] =
{137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,0,0,0,10,0,0,0,4,8,6,0,0,0,183,56,174,205,0,0,0,42,73,68,65
,84,120,1,99,248,207,192,240,249,63,195,255,207,12,64,12,228,128,49,54,49,6,32,0,18,216,20,2,217,72,152,120,19,177,233,198,102,11,0,141
,224,73,213,85,122,202,221,0,0,0,0,73,69,78,68,174,66,96,130};
}

class CrtShadersGeomDeluxeMasksSlot_2_5x4_bgrTextureDef : public TextureDef
{
public:
	CrtShadersGeomDeluxeMasksSlot_2_5x4_bgrTextureDef() : TextureDef{}
	{
		Name = "slot_2_5x4_bgr.png";
		Data = CrtShadersGeomDeluxeMasksSlot_2_5x4_bgrTextureDefs::sData;
		DataLength = sizeof(CrtShadersGeomDeluxeMasksSlot_2_5x4_bgrTextureDefs::sData);
	}
};
