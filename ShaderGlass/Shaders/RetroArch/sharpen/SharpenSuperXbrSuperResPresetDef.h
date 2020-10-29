/*
ShaderGlass preset sharpen / super-xbr-super-res imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/sharpen/super-xbr-super-res.slangp
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
         	ShaderDefs.push_back(XbrShadersSuperXbrSuperXbrPass0ShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.000000")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(XbrShadersSuperXbrSuperXbrPass1ShaderDef()
.Param("filter_linear", "false")
.Param("scale", "2.000000")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(XbrShadersSuperXbrSuperXbrPass2ShaderDef()
.Param("alias", "REF")
.Param("filter_linear", "false")
.Param("scale", "1.000000")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(CubicShadersCubicShaderDef()
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
         	ShaderDefs.push_back(MiscDeposterizePass0ShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.000000")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(MiscDeposterizePass1ShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.000000")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(XbrShadersSuperXbrCustomJinc2SharperShaderDef()
.Param("filter_linear", "false"));
	}
};
}
