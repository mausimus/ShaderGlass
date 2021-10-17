/*
ShaderGlass preset handheld / psp-color imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/handheld/psp-color.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class HandheldPspColorPresetDef : public PresetDef
{
public:
	HandheldPspColorPresetDef() : PresetDef{}
	{
		Name = "psp-color";
		Category = "handheld";
	}

	virtual void Build() {
         	ShaderDefs.push_back(HandheldShadersColorPspColorShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
	}
};
}
