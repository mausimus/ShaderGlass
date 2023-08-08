/*
ShaderGlass preset ddt / ddt-xbr-lv1 imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/6f921ee4815a7894a33855974285b04545a4fa42/ddt/ddt-xbr-lv1.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class DdtDdtXbrLv1PresetDef : public PresetDef
{
public:
	DdtDdtXbrLv1PresetDef() : PresetDef{}
	{
		Name = "ddt-xbr-lv1";
		Category = "ddt";
	}

	virtual void Build() {
         	ShaderDefs.push_back(DdtShadersDdtXbrLv1ShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "viewport"));
	}
};
}
