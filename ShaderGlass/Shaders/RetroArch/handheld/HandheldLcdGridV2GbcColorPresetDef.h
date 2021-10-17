/*
ShaderGlass preset handheld / lcd-grid-v2-gbc-color imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/handheld/lcd-grid-v2-gbc-color.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class HandheldLcdGridV2GbcColorPresetDef : public PresetDef
{
public:
	HandheldLcdGridV2GbcColorPresetDef() : PresetDef{}
	{
		Name = "lcd-grid-v2-gbc-color";
		Category = "handheld";
	}

	virtual void Build() {
         	ShaderDefs.push_back(HandheldShadersLcdCgwgLcdGridV2ShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "viewport"));
         	ShaderDefs.push_back(HandheldShadersColorGbcColorShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
	}
};
}
