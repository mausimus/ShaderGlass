/*
ShaderGlass preset sabr / sabr-hybrid-deposterize imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/6f921ee4815a7894a33855974285b04545a4fa42/sabr/sabr-hybrid-deposterize.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class SabrSabrHybridDeposterizePresetDef : public PresetDef
{
public:
	SabrSabrHybridDeposterizePresetDef() : PresetDef{}
	{
		Name = "sabr-hybrid-deposterize";
		Category = "sabr";
	}

	virtual void Build() {
         	ShaderDefs.push_back(SabrShadersSabrHybridDeposterizeShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "viewport"));
	}
};
}
