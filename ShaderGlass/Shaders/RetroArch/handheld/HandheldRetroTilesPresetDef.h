/*
ShaderGlass preset handheld / retro-tiles imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/handheld/retro-tiles.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class HandheldRetroTilesPresetDef : public PresetDef
{
public:
	HandheldRetroTilesPresetDef() : PresetDef{}
	{
		Name = "retro-tiles";
		Category = "handheld";
	}

	virtual void Build() {
         	ShaderDefs.push_back(HandheldShadersRetroTilesShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "viewport"));
	}
};
}
