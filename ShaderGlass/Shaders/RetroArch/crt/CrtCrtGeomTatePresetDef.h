/*
ShaderGlass preset crt / crt-geom-tate imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/crt/crt-geom-tate.slangp
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
.Param("filter_linear", "false"));
	}
};
}
