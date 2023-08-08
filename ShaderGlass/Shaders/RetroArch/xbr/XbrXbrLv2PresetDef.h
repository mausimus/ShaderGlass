/*
ShaderGlass preset xbr / xbr-lv2 imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/6f921ee4815a7894a33855974285b04545a4fa42/xbr/xbr-lv2.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class XbrXbrLv2PresetDef : public PresetDef
{
public:
	XbrXbrLv2PresetDef() : PresetDef{}
	{
		Name = "xbr-lv2";
		Category = "xbr";
	}

	virtual void Build() {
         	ShaderDefs.push_back(XbrShadersSupportLinearizeShaderDef()
.Param("alias", "XbrSource")
.Param("filter_linear", "false")
.Param("scale", "1.000000")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(XbrShadersXbrLv2MultipassXbrLv2Pass0ShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.000000")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(XbrShadersXbrLv2MultipassXbrLv2Pass1ShaderDef()
.Param("filter_linear", "false")
.Param("scale", "3.000000")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(XbrShadersSupportBSplineXShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type_x", "viewport")
.Param("scale_type_y", "source"));
         	ShaderDefs.push_back(XbrShadersSupportBSplineYShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "viewport"));
         	ShaderDefs.push_back(XbrShadersSupportDelinearizeShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.000000")
.Param("scale_type", "source"));
	}
};
}
