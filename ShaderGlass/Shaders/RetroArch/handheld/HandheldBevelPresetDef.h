/*
ShaderGlass preset handheld / bevel imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/handheld/bevel.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class HandheldBevelPresetDef : public PresetDef
{
public:
	HandheldBevelPresetDef() : PresetDef{}
	{
		Name = "bevel";
		Category = "handheld";
	}

	virtual void Build() {
         	ShaderDefs.push_back(HandheldShadersBevelShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "viewport"));
	}
};
}
