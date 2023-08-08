/*
ShaderGlass preset crt / yeetron imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/6f921ee4815a7894a33855974285b04545a4fa42/crt/yeetron.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class CrtYeetronPresetDef : public PresetDef
{
public:
	CrtYeetronPresetDef() : PresetDef{}
	{
		Name = "yeetron";
		Category = "crt";
	}

	virtual void Build() {
         	ShaderDefs.push_back(CrtShadersYeetronShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "viewport"));
	}
};
}
