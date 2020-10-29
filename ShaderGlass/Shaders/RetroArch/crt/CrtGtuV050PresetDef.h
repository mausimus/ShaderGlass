/*
ShaderGlass preset crt / gtu-v050 imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/crt/gtu-v050.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class CrtGtuV050PresetDef : public PresetDef
{
public:
	CrtGtuV050PresetDef() : PresetDef{}
	{
		Name = "gtu-v050";
		Category = "crt";
	}

	virtual void Build() {
         	ShaderDefs.push_back(CrtShadersGtuV050Pass1ShaderDef()
.Param("float_framebuffer", "true")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(CrtShadersGtuV050Pass2ShaderDef()
.Param("filter_linear", "true")
.Param("float_framebuffer", "true")
.Param("scale_type_x", "viewport")
.Param("scale_type_y", "source")
.Param("scale_x", "1.0")
.Param("scale_y", "1.0"));
         	ShaderDefs.push_back(CrtShadersGtuV050Pass3ShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "viewport"));
	}
};
}
