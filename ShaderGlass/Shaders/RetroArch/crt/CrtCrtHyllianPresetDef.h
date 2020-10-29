/*
ShaderGlass preset crt / crt-hyllian imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/crt/crt-hyllian.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class CrtCrtHyllianPresetDef : public PresetDef
{
public:
	CrtCrtHyllianPresetDef() : PresetDef{}
	{
		Name = "crt-hyllian";
		Category = "crt";
	}

	virtual void Build() {
         	ShaderDefs.push_back(CrtShadersCrtHyllianShaderDef()
.Param("filter_linear", "false"));
	}
};
}
