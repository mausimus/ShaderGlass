/*
ShaderGlass preset xbr-other-presets / xbr-lv3-9x-standalone imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/6f921ee4815a7894a33855974285b04545a4fa42/xbr/other presets/xbr-lv3-9x-standalone.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class XbrOtherPresetsXbrLv39xStandalonePresetDef : public PresetDef
{
public:
	XbrOtherPresetsXbrLv39xStandalonePresetDef() : PresetDef{}
	{
		Name = "xbr-lv3-9x-standalone";
		Category = "xbr-other-presets";
	}

	virtual void Build() {
         	ShaderDefs.push_back(XbrShadersXbrLv3StandaloneShaderDef()
.Param("filter_linear", "false")
.Param("scale", "3.000000")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(XbrShadersXbrLv3StandaloneShaderDef()
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
	}
};
}
