/*
ShaderGlass preset misc / convergence imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/misc/convergence.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class MiscConvergencePresetDef : public PresetDef
{
public:
	MiscConvergencePresetDef() : PresetDef{}
	{
		Name = "convergence";
		Category = "misc";
	}

	virtual void Build() {
         	ShaderDefs.push_back(MiscShadersConvergenceShaderDef()
.Param("filter_linear", "true")
.Param("scale_type", "source"));
	}
};
}
