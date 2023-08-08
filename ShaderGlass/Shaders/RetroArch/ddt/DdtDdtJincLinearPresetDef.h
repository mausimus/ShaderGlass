/*
ShaderGlass preset ddt / ddt-jinc-linear imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/6f921ee4815a7894a33855974285b04545a4fa42/ddt/ddt-jinc-linear.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class DdtDdtJincLinearPresetDef : public PresetDef
{
public:
	DdtDdtJincLinearPresetDef() : PresetDef{}
	{
		Name = "ddt-jinc-linear";
		Category = "ddt";
	}

	virtual void Build() {
         	ShaderDefs.push_back(XbrShadersSupportLinearizeShaderDef()
.Param("alias", "XbrSource")
.Param("filter_linear", "false")
.Param("scale", "1.000000")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(DdtShadersDdtJincShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.000000")
.Param("scale_type", "viewport"));
         	ShaderDefs.push_back(XbrShadersSupportDelinearizeShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "source"));
	}
};
}
