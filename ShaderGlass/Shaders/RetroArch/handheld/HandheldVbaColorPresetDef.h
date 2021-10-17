/*
ShaderGlass preset handheld / vba-color imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/handheld/vba-color.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class HandheldVbaColorPresetDef : public PresetDef
{
public:
	HandheldVbaColorPresetDef() : PresetDef{}
	{
		Name = "vba-color";
		Category = "handheld";
	}

	virtual void Build() {
         	ShaderDefs.push_back(HandheldShadersColorVbaColorShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
	}
};
}
