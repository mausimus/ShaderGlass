/*
ShaderGlass preset crt / crt-hyllian-3d imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/crt/crt-hyllian-3d.slangp
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
.Param("filter_linear", "true")
.Param("scale_type", "viewport"));
	}
};
}
