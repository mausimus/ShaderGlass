/*
ShaderGlass preset crt / fakelottes imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/crt/fakelottes.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class CrtFakelottesPresetDef : public PresetDef
{
public:
	CrtFakelottesPresetDef() : PresetDef{}
	{
		Name = "fakelottes";
		Category = "crt";
	}

	virtual void Build() {
         	ShaderDefs.push_back(CrtShadersFakelottesShaderDef()
.Param("filter_linear", "true"));
	}
};
}
