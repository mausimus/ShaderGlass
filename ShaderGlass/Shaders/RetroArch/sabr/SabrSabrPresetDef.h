/*
ShaderGlass preset sabr / sabr imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/6f921ee4815a7894a33855974285b04545a4fa42/sabr/sabr.slangp
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
.Param("filter_linear", "false")
.Param("scale_type", "viewport"));
	}
};
}
