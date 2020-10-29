/*
ShaderGlass preset xbr / xbr-lv2-3d imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/xbr/xbr-lv2-3d.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class XbrXbrLv23dPresetDef : public PresetDef
{
public:
	XbrXbrLv23dPresetDef() : PresetDef{}
	{
		Name = "xbr-lv2-3d";
		Category = "xbr";
	}

	virtual void Build() {
         	ShaderDefs.push_back(XbrShadersXbrLv23dShaderDef()
.Param("filter_linear", "false"));
	}
};
}
