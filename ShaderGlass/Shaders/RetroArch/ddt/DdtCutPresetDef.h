/*
ShaderGlass preset ddt / cut imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/6f921ee4815a7894a33855974285b04545a4fa42/ddt/cut.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class DdtCutPresetDef : public PresetDef
{
public:
	DdtCutPresetDef() : PresetDef{}
	{
		Name = "cut";
		Category = "ddt";
	}

	virtual void Build() {
         	ShaderDefs.push_back(DdtShadersCutShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "viewport"));
	}
};
}
