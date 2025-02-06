/*
ShaderGlass preset crt / crt-lottes-fast imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/crt/crt-lottes-fast.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class CrtCrtLottesFastPresetDef : public PresetDef
{
public:
	CrtCrtLottesFastPresetDef() : PresetDef{}
	{
		Name = "crt-lottes-fast";
		Category = "crt";
	}

	virtual void Build() {
         	ShaderDefs.push_back(CrtShadersCrtLottesFastShaderDef()
.Param("filter_linear", "true")
.Param("scale_type", "viewport"));
	}
};
}
