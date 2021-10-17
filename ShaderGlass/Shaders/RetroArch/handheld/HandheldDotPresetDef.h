/*
ShaderGlass preset handheld / dot imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/handheld/dot.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class HandheldDotPresetDef : public PresetDef
{
public:
	HandheldDotPresetDef() : PresetDef{}
	{
		Name = "dot";
		Category = "handheld";
	}

	virtual void Build() {
         	ShaderDefs.push_back(HandheldShadersDotShaderDef()
.Param("filter_linear", "false"));
	}
};
}
