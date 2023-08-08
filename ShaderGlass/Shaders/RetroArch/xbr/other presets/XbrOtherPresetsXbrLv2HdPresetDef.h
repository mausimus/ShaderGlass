/*
ShaderGlass preset xbr-other-presets / xbr-lv2-hd imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/6f921ee4815a7894a33855974285b04545a4fa42/xbr/other presets/xbr-lv2-hd.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class XbrOtherPresetsXbrLv2HdPresetDef : public PresetDef
{
public:
	XbrOtherPresetsXbrLv2HdPresetDef() : PresetDef{}
	{
		Name = "xbr-lv2-hd";
		Category = "xbr-other-presets";
	}

	virtual void Build() {
         	ShaderDefs.push_back(XbrShadersXbrLv2HdShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "viewport"));
	}
};
}
