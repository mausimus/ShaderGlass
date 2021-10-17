/*
ShaderGlass preset handheld / lcd1x_nds imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/handheld/lcd1x_nds.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class HandheldLcd1x_ndsPresetDef : public PresetDef
{
public:
	HandheldLcd1x_ndsPresetDef() : PresetDef{}
	{
		Name = "lcd1x_nds";
		Category = "handheld";
	}

	virtual void Build() {
         	ShaderDefs.push_back(HandheldShadersLcd1x_ndsShaderDef()
.Param("filter_linear", "false")
.Param("float_framebuffer", "false")
.Param("mipmap_input", "false")
.Param("srgb_framebuffer", "false"));
	}
};
}
