/*
ShaderGlass preset crt / crt-geom-deluxe imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/crt/crt-geom-deluxe.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class CrtCrtGeomDeluxePresetDef : public PresetDef
{
public:
	CrtCrtGeomDeluxePresetDef() : PresetDef{}
	{
		Name = "crt-geom-deluxe";
		Category = "crt";
	}

	virtual void Build() {
         	ShaderDefs.push_back(CrtShadersGeomDeluxePhosphor_applyShaderDef()
.Param("alias", "internal1"));
         	ShaderDefs.push_back(CrtShadersGeomDeluxePhosphor_updateShaderDef()
.Param("alias", "phosphor"));
         	ShaderDefs.push_back(CrtShadersGeomDeluxeGaussxShaderDef()
.Param("alias", "internal2")
.Param("filter_linear", "true"));
         	ShaderDefs.push_back(CrtShadersGeomDeluxeGaussyShaderDef()
.Param("alias", "blur")
.Param("filter_linear", "true"));
         	ShaderDefs.push_back(CrtShadersGeomDeluxeCrtGeomDeluxeShaderDef()
.Param("filter_linear", "true"));
	}
};
}
