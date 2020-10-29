/*
ShaderGlass preset ddt / ddt imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/ddt/ddt.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class DdtDdtPresetDef : public PresetDef
{
public:
	DdtDdtPresetDef() : PresetDef{}
	{
		Name = "ddt";
		Category = "ddt";
	}

	virtual void Build() {
         	ShaderDefs.push_back(DdtShadersDdtShaderDef()
.Param("filter_linear", "false"));
	}
};
}
