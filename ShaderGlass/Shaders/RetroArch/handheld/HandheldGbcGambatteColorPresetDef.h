/*
ShaderGlass preset handheld / gbc-gambatte-color imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/handheld/gbc-gambatte-color.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class HandheldGbcGambatteColorPresetDef : public PresetDef
{
public:
	HandheldGbcGambatteColorPresetDef() : PresetDef{}
	{
		Name = "gbc-gambatte-color";
		Category = "handheld";
	}

	virtual void Build() {
         	ShaderDefs.push_back(HandheldShadersColorGbcGambatteColorShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
	}
};
}
