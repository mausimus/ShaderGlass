/*
ShaderGlass preset crt / crt-hyllian-fast imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/crt/crt-hyllian-fast.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class CrtCrtHyllianFastPresetDef : public PresetDef
{
public:
	CrtCrtHyllianFastPresetDef() : PresetDef{}
	{
		Name = "crt-hyllian-fast";
		Category = "crt";
	}

	virtual void Build() {
         	ShaderDefs.push_back(CrtShadersHyllianCrtHyllianFastShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "viewport"));
	}
};
}
