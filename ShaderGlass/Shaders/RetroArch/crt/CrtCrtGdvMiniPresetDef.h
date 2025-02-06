/*
ShaderGlass preset crt / crt-gdv-mini imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/crt/crt-gdv-mini.slangp
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
