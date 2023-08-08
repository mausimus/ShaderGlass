/*
ShaderGlass preset crt / fake-crt-geom-potato imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/6f921ee4815a7894a33855974285b04545a4fa42/crt/fake-crt-geom-potato.slangp
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
.Param("filter_linear", "false")
.Param("scale_type", "viewport"));
	}
};
}
