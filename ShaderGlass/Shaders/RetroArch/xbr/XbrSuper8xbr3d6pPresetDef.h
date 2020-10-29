/*
ShaderGlass preset xbr / super-8xbr-3d-6p imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/xbr/super-8xbr-3d-6p.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class XbrSuper8xbr3d6pPresetDef : public PresetDef
{
public:
	XbrSuper8xbr3d6pPresetDef() : PresetDef{}
	{
		Name = "super-8xbr-3d-6p";
		Category = "xbr";
	}

	virtual void Build() {
         	ShaderDefs.push_back(XbrShadersSuperXbrSuper8xbr3dPass0ShaderDef()
.Param("filter_linear", "false")
.Param("scale_type_x", "source")
.Param("scale_type_y", "source")
.Param("scale_x", "1.000000")
.Param("scale_y", "1.000000"));
         	ShaderDefs.push_back(XbrShadersSuperXbrSuper8xbr3dPass1ShaderDef()
.Param("filter_linear", "false")
.Param("scale_type_x", "source")
.Param("scale_type_y", "source")
.Param("scale_x", "1.000000")
.Param("scale_y", "1.000000"));
         	ShaderDefs.push_back(XbrShadersSuperXbrSuper8xbr3dPass2ShaderDef()
.Param("filter_linear", "false")
.Param("scale_type_x", "source")
.Param("scale_type_y", "source")
.Param("scale_x", "1.000000")
.Param("scale_y", "1.000000"));
         	ShaderDefs.push_back(XbrShadersSuperXbrSuper8xbr3dPass3ShaderDef()
.Param("filter_linear", "false")
.Param("scale_type_x", "source")
.Param("scale_type_y", "source")
.Param("scale_x", "1.000000")
.Param("scale_y", "1.000000"));
         	ShaderDefs.push_back(XbrShadersSuperXbrSuper8xbr3dPass4ShaderDef()
.Param("filter_linear", "false")
.Param("scale_type_x", "source")
.Param("scale_type_y", "source")
.Param("scale_x", "1.000000")
.Param("scale_y", "1.000000"));
         	ShaderDefs.push_back(XbrShadersSuperXbrSuper8xbr3dPass5ShaderDef()
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
