/*
ShaderGlass texture reshade-shaders-LUT / 32 imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/6f921ee4815a7894a33855974285b04545a4fa42/reshade/shaders/LUT/32.png
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace ReshadeShadersLUT32TextureDefs
{
const BYTE sData[] =
{137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,0,0,4,0,0,0,0,32,8,2,0,0,0,3,156,157,241,0,0,0,217,73,68,65
,84,120,1,237,219,5,14,133,64,12,5,192,226,112,255,3,227,81,98,184,205,124,23,172,27,123,105,154,71,68,61,222,155,241,121,249,126,254,184,245
,111,73,212,47,223,127,163,62,234,255,153,253,91,95,251,87,127,245,183,127,235,171,254,234,147,247,79,191,0,0,0,252,42,0,0,0,0,121,52,113
,17,0,0,64,7,0,0,0,16,0,0,0,0,1,0,0,0,48,3,0,0,0,232,0,0,0,0,2,0,0,0,8,0,0,0,128,0,0,0,0
,24,2,6,0,0,116,0,0,0,0,1,0,0,0,16,0,0,0,0,51,0,0,0,128,14,0,0,0,32,0,0,0,0,2,0,0,0,96,6,0
,0,0,116,0,0,0,0,1,0,0,0,16,0,0,0,0,1,0,0,0,48,4,12,0,0,232,0,0,0,0,2,0,0,0,32,0,0,0,0,102
,0,0,0,0,29,0,0,0,64,0,0,0,0,58,203,69,34,126,159,229,25,203,0,0,0,0,73,69,78,68,174,66,96,130};
}

class ReshadeShadersLUT32TextureDef : public TextureDef
{
public:
	ReshadeShadersLUT32TextureDef() : TextureDef{}
	{
		Name = "32.png";
		Data = ReshadeShadersLUT32TextureDefs::sData;
		DataLength = sizeof(ReshadeShadersLUT32TextureDefs::sData);
	}
};
