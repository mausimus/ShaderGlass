/*
ShaderGlass preset crt / crt-gdv-mini imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/6f921ee4815a7894a33855974285b04545a4fa42/crt/crt-gdv-mini.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class CrtCrtGdvMiniPresetDef : public PresetDef
{
public:
	CrtCrtGdvMiniPresetDef() : PresetDef{}
	{
		Name = "crt-gdv-mini";
		Category = "crt";
	}

	virtual void Build() {
         	ShaderDefs.push_back(CrtShadersCrtGdvMiniShaderDef()
.Param("scale_type", "viewport"));
	}
};
}
