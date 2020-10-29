/*
ShaderGlass preset sabr / sabr imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/sabr/sabr.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class SabrSabrPresetDef : public PresetDef
{
public:
	SabrSabrPresetDef() : PresetDef{}
	{
		Name = "sabr";
		Category = "sabr";
	}

	virtual void Build() {
         	ShaderDefs.push_back(SabrShadersSabrV30ShaderDef()
.Param("filter_linear", "false"));
	}
};
}
