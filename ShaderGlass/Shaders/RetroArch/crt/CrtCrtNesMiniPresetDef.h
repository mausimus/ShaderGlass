/*
ShaderGlass preset crt / crt-nes-mini imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/6f921ee4815a7894a33855974285b04545a4fa42/crt/crt-nes-mini.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class CrtCrtNesMiniPresetDef : public PresetDef
{
public:
	CrtCrtNesMiniPresetDef() : PresetDef{}
	{
		Name = "crt-nes-mini";
		Category = "crt";
	}

	virtual void Build() {
         	ShaderDefs.push_back(CrtShadersCrtNesMiniShaderDef()
.Param("scale_type", "viewport"));
	}
};
}
