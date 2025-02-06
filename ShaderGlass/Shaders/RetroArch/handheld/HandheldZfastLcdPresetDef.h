/*
ShaderGlass preset handheld / zfast-lcd imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/handheld/zfast-lcd.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class HandheldZfastLcdPresetDef : public PresetDef
{
public:
	HandheldZfastLcdPresetDef() : PresetDef{}
	{
		Name = "zfast-lcd";
		Category = "handheld";
	}

	virtual void Build() {
         	ShaderDefs.push_back(HandheldShadersZfast_lcdShaderDef()
.Param("filter_linear", "true")
.Param("scale_type", "viewport"));
	}
};
}
