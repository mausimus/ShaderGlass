/*
ShaderGlass texture reshade-shaders-blendoverlay / grayscale_slotmask imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/reshade/shaders/blendoverlay/grayscale_slotmask.png
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace ReshadeShadersBlendoverlayGrayscale_slotmaskTextureDefs
{
const BYTE sData[] =
{137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,0,0,0,6,0,0,0,4,8,2,0,0,0,34,102,217,20,0,0,0,50,73,68,65
,84,8,215,61,202,65,17,0,48,8,3,193,235,76,229,129,13,60,161,131,8,228,1,109,158,155,59,17,193,91,102,2,23,48,51,64,82,85,45,185
,251,144,164,165,57,127,222,94,212,16,64,157,51,108,33,0,0,0,0,73,69,78,68,174,66,96,130};
}

class ReshadeShadersBlendoverlayGrayscale_slotmaskTextureDef : public TextureDef
{
public:
	ReshadeShadersBlendoverlayGrayscale_slotmaskTextureDef() : TextureDef{}
	{
		Data = ReshadeShadersBlendoverlayGrayscale_slotmaskTextureDefs::sData;
		DataLength = sizeof(ReshadeShadersBlendoverlayGrayscale_slotmaskTextureDefs::sData);
	}
};
