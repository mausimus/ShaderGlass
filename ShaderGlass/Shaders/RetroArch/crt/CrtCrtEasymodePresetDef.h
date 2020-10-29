/*
ShaderGlass preset crt / crt-easymode imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/crt/crt-easymode.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class CrtCrtEasymodePresetDef : public PresetDef
{
public:
	CrtCrtEasymodePresetDef() : PresetDef{}
	{
		Name = "crt-easymode";
		Category = "crt";
	}

	virtual void Build() {
         	ShaderDefs.push_back(CrtShadersCrtEasymodeShaderDef()
.Param("filter_linear", "false"));
	}
};
}
