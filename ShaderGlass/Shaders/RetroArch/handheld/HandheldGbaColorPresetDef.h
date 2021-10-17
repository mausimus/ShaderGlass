/*
ShaderGlass preset handheld / gba-color imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/handheld/gba-color.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class HandheldGbaColorPresetDef : public PresetDef
{
public:
	HandheldGbaColorPresetDef() : PresetDef{}
	{
		Name = "gba-color";
		Category = "handheld";
	}

	virtual void Build() {
         	ShaderDefs.push_back(HandheldShadersColorGbaColorShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
	}
};
}
