/*
ShaderGlass preset crt / crt-frutbunn imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/crt/crt-frutbunn.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class CrtCrtFrutbunnPresetDef : public PresetDef
{
public:
	CrtCrtFrutbunnPresetDef() : PresetDef{}
	{
		Name = "crt-frutbunn";
		Category = "crt";
	}

	virtual void Build() {
         	ShaderDefs.push_back(CrtShadersCrtFrutbunnShaderDef()
.Param("scale_type", "viewport"));
	}
};
}
