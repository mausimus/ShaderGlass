/*
ShaderGlass preset crt / crt-caligari imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/crt/crt-caligari.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class CrtCrtCaligariPresetDef : public PresetDef
{
public:
	CrtCrtCaligariPresetDef() : PresetDef{}
	{
		Name = "crt-caligari";
		Category = "crt";
	}

	virtual void Build() {
         	ShaderDefs.push_back(CrtShadersCrtCaligariShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "viewport"));
	}
};
}
