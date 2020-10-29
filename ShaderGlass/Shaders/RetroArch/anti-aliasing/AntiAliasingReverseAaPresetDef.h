/*
ShaderGlass preset anti-aliasing / reverse-aa imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/anti-aliasing/reverse-aa.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class AntiAliasingReverseAaPresetDef : public PresetDef
{
public:
	AntiAliasingReverseAaPresetDef() : PresetDef{}
	{
		Name = "reverse-aa";
		Category = "anti-aliasing";
	}

	virtual void Build() {
         	ShaderDefs.push_back(AntiAliasingShadersReverseAaShaderDef()
.Param("filter_linear", "false")
.Param("scale", "2.0")
.Param("scale_type", "source"));
	}
};
}
