/*
ShaderGlass preset ddt / ddt imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/6f921ee4815a7894a33855974285b04545a4fa42/ddt/ddt.slangp
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
.Param("filter_linear", "false")
.Param("scale_type", "viewport"));
	}
};
}
