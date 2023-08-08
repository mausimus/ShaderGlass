/*
ShaderGlass preset crt / crt-geom-tate imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/6f921ee4815a7894a33855974285b04545a4fa42/crt/crt-geom-tate.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class CrtCrtGeomTatePresetDef : public PresetDef
{
public:
	CrtCrtGeomTatePresetDef() : PresetDef{}
	{
		Name = "crt-geom-tate";
		Category = "crt";
	}

	virtual void Build() {
         	ShaderDefs.push_back(CrtShadersCrtGeomShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "viewport"));
	}
};
}
