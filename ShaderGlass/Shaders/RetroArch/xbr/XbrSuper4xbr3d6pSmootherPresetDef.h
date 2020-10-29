/*
ShaderGlass preset xbr / super-4xbr-3d-6p-smoother imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/xbr/super-4xbr-3d-6p-smoother.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class XbrSuper4xbr3d6pSmootherPresetDef : public PresetDef
{
public:
	XbrSuper4xbr3d6pSmootherPresetDef() : PresetDef{}
	{
		Name = "super-4xbr-3d-6p-smoother";
		Category = "xbr";
	}

	virtual void Build() {
         	ShaderDefs.push_back(XbrShadersSuperXbrSuper4xbr3dPass0ShaderDef()
.Param("filter_linear", "false")
.Param("scale_type_x", "source")
.Param("scale_type_y", "source")
.Param("scale_x", "1.000000")
.Param("scale_y", "1.000000"));
         	ShaderDefs.push_back(XbrShadersSuperXbrSuper4xbr3dPass1ShaderDef()
.Param("filter_linear", "false")
.Param("scale_type_x", "source")
.Param("scale_type_y", "source")
.Param("scale_x", "1.000000")
.Param("scale_y", "1.000000"));
         	ShaderDefs.push_back(XbrShadersSuperXbrSuper4xbr3dPass1fShaderDef()
.Param("filter_linear", "false")
.Param("scale_type_x", "source")
.Param("scale_type_y", "source")
.Param("scale_x", "1.000000")
.Param("scale_y", "1.000000"));
         	ShaderDefs.push_back(XbrShadersSuperXbrSuper4xbr3dPass2ShaderDef()
.Param("filter_linear", "false")
.Param("scale_type_x", "source")
.Param("scale_type_y", "source")
.Param("scale_x", "1.000000")
.Param("scale_y", "1.000000"));
         	ShaderDefs.push_back(XbrShadersSuperXbrSuper4xbr3dPass3ShaderDef()
.Param("filter_linear", "false")
.Param("scale_type_x", "source")
.Param("scale_type_y", "source")
.Param("scale_x", "1.000000")
.Param("scale_y", "1.000000"));
         	ShaderDefs.push_back(XbrShadersSuperXbrSuper4xbr3dPass3fShaderDef()
.Param("filter_linear", "false")
.Param("scale_type_x", "source")
.Param("scale_type_y", "source")
.Param("scale_x", "1.000000")
.Param("scale_y", "1.000000"));
         	ShaderDefs.push_back(StockStockShaderDef()
.Param("filter_linear", "true"));
	}
};
}
