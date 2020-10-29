/*
ShaderGlass preset crt / crt-pi imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/crt/crt-pi.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class CrtCrtPiPresetDef : public PresetDef
{
public:
	CrtCrtPiPresetDef() : PresetDef{}
	{
		Name = "crt-pi";
		Category = "crt";
	}

	virtual void Build() {
         	ShaderDefs.push_back(CrtShadersCrtPiShaderDef()
.Param("filter_linear", "false"));
	}
};
}
