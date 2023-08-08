/*
ShaderGlass preset crt / crt-mattias imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/6f921ee4815a7894a33855974285b04545a4fa42/crt/crt-mattias.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class CrtCrtMattiasPresetDef : public PresetDef
{
public:
	CrtCrtMattiasPresetDef() : PresetDef{}
	{
		Name = "crt-mattias";
		Category = "crt";
	}

	virtual void Build() {
         	ShaderDefs.push_back(CrtShadersCrtMattiasShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "viewport"));
	}
};
}
