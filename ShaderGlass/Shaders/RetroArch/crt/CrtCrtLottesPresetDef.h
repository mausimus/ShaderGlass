/*
ShaderGlass preset crt / crt-lottes imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/crt/crt-lottes.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class CrtCrtLottesPresetDef : public PresetDef
{
public:
	CrtCrtLottesPresetDef() : PresetDef{}
	{
		Name = "crt-lottes";
		Category = "crt";
	}

	virtual void Build() {
         	ShaderDefs.push_back(CrtShadersCrtLottesShaderDef()
.Param("filter_linear", "false"));
	}
};
}
