/*
ShaderGlass preset handheld / lcd3x imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/handheld/lcd3x.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class HandheldLcd3xPresetDef : public PresetDef
{
public:
	HandheldLcd3xPresetDef() : PresetDef{}
	{
		Name = "lcd3x";
		Category = "handheld";
	}

	virtual void Build() {
         	ShaderDefs.push_back(HandheldShadersLcd3xShaderDef()
.Param("filter_linear", "false"));
	}
};
}
