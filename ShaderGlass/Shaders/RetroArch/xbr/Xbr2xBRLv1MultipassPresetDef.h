/*
ShaderGlass preset xbr / 2xBR-lv1-multipass imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/xbr/2xBR-lv1-multipass.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class Xbr2xBRLv1MultipassPresetDef : public PresetDef
{
public:
	Xbr2xBRLv1MultipassPresetDef() : PresetDef{}
	{
		Name = "2xBR-lv1-multipass";
		Category = "xbr";
	}

	virtual void Build() {
         	ShaderDefs.push_back(XbrShaders2xBRMultipass2xbrLv1CPass0ShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(XbrShaders2xBRMultipass2xbrLv1CPass1ShaderDef()
.Param("filter_linear", "false"));
	}
};
}
