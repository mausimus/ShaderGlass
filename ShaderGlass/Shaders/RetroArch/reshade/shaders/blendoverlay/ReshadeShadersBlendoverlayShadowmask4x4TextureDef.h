/*
ShaderGlass texture reshade-shaders-blendoverlay / shadowmask-4x4 imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/6f921ee4815a7894a33855974285b04545a4fa42/reshade/shaders/blendoverlay/shadowmask-4x4.png
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace ReshadeShadersBlendoverlayShadowmask4x4TextureDefs
{
const BYTE sData[] =
{137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,0,0,0,4,0,0,0,4,8,2,0,0,0,38,147,9,41,0,0,0,29,73,68,65
,84,120,1,99,248,41,40,244,224,173,144,208,219,7,66,130,63,81,57,16,10,34,132,194,1,0,108,66,23,217,90,129,15,129,0,0,0,0,73,69
,78,68,174,66,96,130};
}

class ReshadeShadersBlendoverlayShadowmask4x4TextureDef : public TextureDef
{
public:
	ReshadeShadersBlendoverlayShadowmask4x4TextureDef() : TextureDef{}
	{
		Name = "shadowmask-4x4.png";
		Data = ReshadeShadersBlendoverlayShadowmask4x4TextureDefs::sData;
		DataLength = sizeof(ReshadeShadersBlendoverlayShadowmask4x4TextureDefs::sData);
	}
};
