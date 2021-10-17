/*
ShaderGlass preset handheld / nds-color imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/handheld/nds-color.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class HandheldNdsColorPresetDef : public PresetDef
{
public:
	HandheldNdsColorPresetDef() : PresetDef{}
	{
		Name = "nds-color";
		Category = "handheld";
	}

	virtual void Build() {
         	ShaderDefs.push_back(HandheldShadersColorNdsColorShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
	}
};
}
