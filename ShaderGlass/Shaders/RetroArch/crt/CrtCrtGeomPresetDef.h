/*
ShaderGlass preset crt / crt-geom imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/crt/crt-geom.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class CrtCrtGeomPresetDef : public PresetDef
{
public:
	CrtCrtGeomPresetDef() : PresetDef{}
	{
		Name = "crt-geom";
		Category = "crt";
	}

	virtual void Build() {
         	ShaderDefs.push_back(CrtShadersCrtGeomShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "viewport"));
	}
};
}
