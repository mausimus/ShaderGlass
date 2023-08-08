/*
ShaderGlass preset xbr-other-presets / xbr-lv1-standalone imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/6f921ee4815a7894a33855974285b04545a4fa42/xbr/other presets/xbr-lv1-standalone.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class XbrOtherPresetsXbrLv1StandalonePresetDef : public PresetDef
{
public:
	XbrOtherPresetsXbrLv1StandalonePresetDef() : PresetDef{}
	{
		Name = "xbr-lv1-standalone";
		Category = "xbr-other-presets";
	}

	virtual void Build() {
         	ShaderDefs.push_back(XbrShadersXbrLv1StandaloneShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "viewport"));
	}
};
}
