/*
ShaderGlass preset handheld / sameboy-lcd imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/handheld/sameboy-lcd.slangp
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
