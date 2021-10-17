/*
ShaderGlass preset crt / crt-slangtest-cubic imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/crt/crt-slangtest-cubic.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class CrtCrtSlangtestCubicPresetDef : public PresetDef
{
public:
	CrtCrtSlangtestCubicPresetDef() : PresetDef{}
	{
		Name = "crt-slangtest-cubic";
		Category = "crt";
	}

	virtual void Build() {
         	ShaderDefs.push_back(CrtShadersCrtSlangtestLinearizeShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source")
.Param("srgb_framebuffer", "true"));
         	ShaderDefs.push_back(CrtShadersCrtSlangtestCubicShaderDef()
.Param("filter_linear", "false")
.Param("scale_type_x", "viewport")
.Param("scale_type_y", "source")
.Param("scale_x", "1.0")
.Param("scale_y", "1.0")
.Param("srgb_framebuffer", "true"));
         	ShaderDefs.push_back(CrtShadersCrtSlangtestScanlineShaderDef()
.Param("filter_linear", "false"));
	}
};
}
