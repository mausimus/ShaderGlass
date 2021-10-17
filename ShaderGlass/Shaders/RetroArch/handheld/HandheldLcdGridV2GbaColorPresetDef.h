/*
ShaderGlass preset handheld / lcd-grid-v2-gba-color imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/handheld/lcd-grid-v2-gba-color.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class HandheldLcdGridV2GbaColorPresetDef : public PresetDef
{
public:
	HandheldLcdGridV2GbaColorPresetDef() : PresetDef{}
	{
		Name = "lcd-grid-v2-gba-color";
		Category = "handheld";
	}

	virtual void Build() {
         	ShaderDefs.push_back(HandheldShadersLcdCgwgLcdGridV2ShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "viewport"));
         	ShaderDefs.push_back(HandheldShadersColorGbaColorShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
	}
};
}
