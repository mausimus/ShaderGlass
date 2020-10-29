/*
ShaderGlass preset crt / yee64 imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/crt/yee64.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class CrtYee64PresetDef : public PresetDef
{
public:
	CrtYee64PresetDef() : PresetDef{}
	{
		Name = "yee64";
		Category = "crt";
	}

	virtual void Build() {
         	ShaderDefs.push_back(CrtShadersYee64ShaderDef());
	}
};
}
