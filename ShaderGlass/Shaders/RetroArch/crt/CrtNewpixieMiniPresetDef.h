/*
ShaderGlass preset crt / newpixie-mini imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/crt/newpixie-mini.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class CrtNewpixieMiniPresetDef : public PresetDef
{
public:
	CrtNewpixieMiniPresetDef() : PresetDef{}
	{
		Name = "newpixie-mini";
		Category = "crt";
	}

	virtual void Build() {
         	ShaderDefs.push_back(CrtShadersNewpixieMiniNewpixieMiniShaderDef());
	}
};
}
