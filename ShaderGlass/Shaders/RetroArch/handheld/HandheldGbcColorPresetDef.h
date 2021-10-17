/*
ShaderGlass preset handheld / gbc-color imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/handheld/gbc-color.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class HandheldGbcColorPresetDef : public PresetDef
{
public:
	HandheldGbcColorPresetDef() : PresetDef{}
	{
		Name = "gbc-color";
		Category = "handheld";
	}

	virtual void Build() {
         	ShaderDefs.push_back(HandheldShadersColorGbcColorShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
	}
};
}
