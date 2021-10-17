/*
ShaderGlass preset handheld / sameboy-lcd imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/handheld/sameboy-lcd.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class HandheldSameboyLcdPresetDef : public PresetDef
{
public:
	HandheldSameboyLcdPresetDef() : PresetDef{}
	{
		Name = "sameboy-lcd";
		Category = "handheld";
	}

	virtual void Build() {
         	ShaderDefs.push_back(HandheldShadersSameboyLcdShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "viewport"));
	}
};
}
