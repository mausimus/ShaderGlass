/*
ShaderGlass texture reshade/shaders/blendoverlay / grayscale_slotmask imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/reshade/shaders/blendoverlay/grayscale_slotmask.png
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace ReshadeShadersBlendoverlayGrayscale_slotmaskTextureDefs
{
const BYTE sData[] =
{137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,0,0,0,6,0,0,0,4,8,0,0,0,0,136,111,17,159,0,0,0,25,73,68,65
,84,120,1,99,72,3,130,255,255,25,210,54,109,2,34,6,32,19,200,135,242,0,198,96,14,85,113,244,252,187,0,0,0,0,73,69,78,68,174,66
,96,130};
}

class ReshadeShadersBlendoverlayGrayscale_slotmaskTextureDef : public TextureDef
{
public:
	ReshadeShadersBlendoverlayGrayscale_slotmaskTextureDef() : TextureDef{}
	{
		Name = "grayscale_slotmask.png";
		Data = ReshadeShadersBlendoverlayGrayscale_slotmaskTextureDefs::sData;
		DataLength = sizeof(ReshadeShadersBlendoverlayGrayscale_slotmaskTextureDefs::sData);
	}
};
