/*
ShaderGlass preset handheld / dot imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/handheld/dot.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class HandheldDotPresetDef : public PresetDef
{
public:
	HandheldDotPresetDef() : PresetDef{}
	{
		Name = "dot";
		Category = "handheld";
	}

	virtual void Build() {
         	ShaderDefs.push_back(HandheldShadersDotShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "viewport"));
	}
};
}
