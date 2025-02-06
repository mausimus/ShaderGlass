/*
ShaderGlass texture crt-shaders-crtsim / artifacts imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/crt/shaders/crtsim/artifacts.png
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace CrtShadersCrtsimArtifactsTextureDefs
{
const BYTE sData[] =
{137,80,78,71,13,10,26,10,0,0,0,13,73,72,68,82,0,0,1,0,0,0,0,224,2,3,0,0,0,145,47,80,195,0,0,0,9,80,76,84
,69,255,0,0,0,0,255,0,255,0,135,13,30,8,0,0,0,96,73,68,65,84,120,1,237,207,193,13,0,32,8,3,192,142,225,122,44,195,220,14
,193,71,245,146,126,161,189,172,234,81,210,195,15,169,225,7,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4
,4,4,132,203,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,27,68,164,153,3,183,22
,67,182,0,0,0,0,73,69,78,68,174,66,96,130};
}

class CrtShadersCrtsimArtifactsTextureDef : public TextureDef
{
public:
	CrtShadersCrtsimArtifactsTextureDef() : TextureDef{}
	{
		Name = "artifacts.png";
		Data = CrtShadersCrtsimArtifactsTextureDefs::sData;
		DataLength = sizeof(CrtShadersCrtsimArtifactsTextureDefs::sData);
	}
};
