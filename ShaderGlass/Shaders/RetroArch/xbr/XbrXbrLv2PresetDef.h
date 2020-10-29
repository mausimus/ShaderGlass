/*
ShaderGlass preset xbr / xbr-lv2 imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/xbr/xbr-lv2.slangp
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
         	ShaderDefs.push_back(XbrShadersXbrLv2ShaderDef()
.Param("filter_linear", "false"));
	}
};
}
