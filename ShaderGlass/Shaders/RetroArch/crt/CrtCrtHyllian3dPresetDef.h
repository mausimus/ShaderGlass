/*
ShaderGlass preset crt / crt-hyllian-3d imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/crt/crt-hyllian-3d.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class CrtCrtHyllian3dPresetDef : public PresetDef
{
public:
	CrtCrtHyllian3dPresetDef() : PresetDef{}
	{
		Name = "crt-hyllian-3d";
		Category = "crt";
	}

	virtual void Build() {
         	ShaderDefs.push_back(CrtShadersHyllianCrtHyllian3dShaderDef()
.Param("filter_linear", "true"));
	}
};
}
