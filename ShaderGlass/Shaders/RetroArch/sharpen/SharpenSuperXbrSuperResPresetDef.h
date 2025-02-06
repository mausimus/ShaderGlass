/*
ShaderGlass preset sharpen / super-xbr-super-res imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/sharpen/super-xbr-super-res.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class SharpenSuperXbrSuperResPresetDef : public PresetDef
{
public:
	SharpenSuperXbrSuperResPresetDef() : PresetDef{}
	{
		Name = "super-xbr-super-res";
		Category = "sharpen";
	}

	virtual void Build() {
         	ShaderDefs.push_back(StockStockShaderDef()
.Param("alias", "diff_refpass"));
         	ShaderDefs.push_back(StockStockShaderDef()
.Param("alias", "XbrSource"));
         	ShaderDefs.push_back(EdgeSmoothingXbrShadersSuperXbrSuperXbrPass0ShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.000000")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(EdgeSmoothingXbrShadersSuperXbrSuperXbrPass1ShaderDef()
.Param("filter_linear", "false")
.Param("scale", "2.000000")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(EdgeSmoothingXbrShadersSuperXbrSuperXbrPass2ShaderDef()
.Param("alias", "REF")
.Param("filter_linear", "false")
.Param("scale", "1.000000")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(InterpolationShadersCubicShaderDef()
.Param("filter_linear", "false")
.Param("scale", "0.500000")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(SharpenShadersDiffShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.000000")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(SharpenShadersSuperResExShaderDef()
.Param("filter_linear", "true")
.Param("scale", "2.000000")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(MiscShadersDeposterizeDeposterizePass0ShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.000000")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(MiscShadersDeposterizeDeposterizePass1ShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.000000")
.Param("scale_type", "source"));
	}
};
}
