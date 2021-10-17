/*
ShaderGlass preset handheld / bevel imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/handheld/bevel.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class HandheldBevelPresetDef : public PresetDef
{
public:
	HandheldBevelPresetDef() : PresetDef{}
	{
		Name = "bevel";
		Category = "handheld";
	}

	virtual void Build() {
         	ShaderDefs.push_back(HandheldShadersBevelShaderDef()
.Param("filter_linear", "false"));
	}
};
}
