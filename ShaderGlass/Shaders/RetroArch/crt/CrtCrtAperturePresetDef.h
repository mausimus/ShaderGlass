/*
ShaderGlass preset crt / crt-aperture imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/crt/crt-aperture.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class CrtCrtAperturePresetDef : public PresetDef
{
public:
	CrtCrtAperturePresetDef() : PresetDef{}
	{
		Name = "crt-aperture";
		Category = "crt";
	}

	virtual void Build() {
         	ShaderDefs.push_back(CrtShadersCrtApertureShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "viewport"));
	}
};
}
