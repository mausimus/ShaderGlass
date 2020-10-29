/*
ShaderGlass preset xbr / xbr-lv3 imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/xbr/xbr-lv3.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class XbrXbrLv3PresetDef : public PresetDef
{
public:
	XbrXbrLv3PresetDef() : PresetDef{}
	{
		Name = "xbr-lv3";
		Category = "xbr";
	}

	virtual void Build() {
         	ShaderDefs.push_back(XbrShadersXbrLv3ShaderDef()
.Param("filter_linear", "false"));
	}
};
}
