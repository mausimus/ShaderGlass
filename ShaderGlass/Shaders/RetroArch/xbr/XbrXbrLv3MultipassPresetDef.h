/*
ShaderGlass preset xbr / xbr-lv3-multipass imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/xbr/xbr-lv3-multipass.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class XbrXbrLv3MultipassPresetDef : public PresetDef
{
public:
	XbrXbrLv3MultipassPresetDef() : PresetDef{}
	{
		Name = "xbr-lv3-multipass";
		Category = "xbr";
	}

	virtual void Build() {
         	ShaderDefs.push_back(XbrShadersXbrLv3MultipassXbrLv3Pass0ShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(XbrShadersXbrLv3MultipassXbrLv3Pass1ShaderDef()
.Param("filter_linear", "false"));
	}
};
}
