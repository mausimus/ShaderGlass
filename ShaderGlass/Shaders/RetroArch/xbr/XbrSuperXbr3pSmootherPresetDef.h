/*
ShaderGlass preset xbr / super-xbr-3p-smoother imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/xbr/super-xbr-3p-smoother.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class XbrSuperXbr3pSmootherPresetDef : public PresetDef
{
public:
	XbrSuperXbr3pSmootherPresetDef() : PresetDef{}
	{
		Name = "super-xbr-3p-smoother";
		Category = "xbr";
	}

	virtual void Build() {
         	ShaderDefs.push_back(XbrShadersSuperXbrSuperXbrPass0ShaderDef()
.Param("filter_linear", "false")
.Param("scale_type_x", "source")
.Param("scale_type_y", "source")
.Param("scale_x", "1.000000")
.Param("scale_y", "1.000000"));
         	ShaderDefs.push_back(XbrShadersSuperXbrSuperXbrPass1ShaderDef()
.Param("filter_linear", "false")
.Param("scale_type_x", "source")
.Param("scale_type_y", "source")
.Param("scale_x", "2.000000")
.Param("scale_y", "2.000000"));
         	ShaderDefs.push_back(XbrShadersSuperXbrSuperXbrPass2ShaderDef()
.Param("filter_linear", "false")
.Param("scale_type_x", "source")
.Param("scale_type_y", "source")
.Param("scale_x", "1.000000")
.Param("scale_y", "1.000000"));
         	ShaderDefs.push_back(XbrShadersSuperXbrCustomJinc2SharperShaderDef()
.Param("filter_linear", "false"));
	}
};
}
