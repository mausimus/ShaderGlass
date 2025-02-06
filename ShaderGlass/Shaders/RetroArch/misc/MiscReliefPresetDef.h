/*
ShaderGlass preset misc / relief imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/misc/relief.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class MiscReliefPresetDef : public PresetDef
{
public:
	MiscReliefPresetDef() : PresetDef{}
	{
		Name = "relief";
		Category = "misc";
	}

	virtual void Build() {
         	ShaderDefs.push_back(MiscShadersReliefShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "viewport"));
	}
};
}
