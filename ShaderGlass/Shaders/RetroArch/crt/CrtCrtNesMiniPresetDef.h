/*
ShaderGlass preset crt / crt-nes-mini imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/crt/crt-nes-mini.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class CrtCrtNesMiniPresetDef : public PresetDef
{
public:
	CrtCrtNesMiniPresetDef() : PresetDef{}
	{
		Name = "crt-nes-mini";
		Category = "crt";
	}

	virtual void Build() {
         	ShaderDefs.push_back(CrtShadersCrtNesMiniShaderDef());
	}
};
}
