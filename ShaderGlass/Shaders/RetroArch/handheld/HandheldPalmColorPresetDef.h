/*
ShaderGlass preset handheld / palm-color imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/handheld/palm-color.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class HandheldPalmColorPresetDef : public PresetDef
{
public:
	HandheldPalmColorPresetDef() : PresetDef{}
	{
		Name = "palm-color";
		Category = "handheld";
	}

	virtual void Build() {
         	ShaderDefs.push_back(HandheldShadersColorPalmColorShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
	}
};
}
