/*
ShaderGlass preset crt / fake-crt-geom-potato imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/crt/fake-crt-geom-potato.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class CrtFakeCrtGeomPotatoPresetDef : public PresetDef
{
public:
	CrtFakeCrtGeomPotatoPresetDef() : PresetDef{}
	{
		Name = "fake-crt-geom-potato";
		Category = "crt";
	}

	virtual void Build() {
         	ShaderDefs.push_back(CrtShadersFakeCrtGeomPotatoShaderDef()
.Param("filter_linear", "true")
.Param("scale_type", "viewport"));
	}
};
}
