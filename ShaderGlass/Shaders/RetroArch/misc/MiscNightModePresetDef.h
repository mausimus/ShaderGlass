/*
ShaderGlass preset misc / night-mode imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/misc/night-mode.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class MiscNightModePresetDef : public PresetDef
{
public:
	MiscNightModePresetDef() : PresetDef{}
	{
		Name = "night-mode";
		Category = "misc";
	}

	virtual void Build() {
         	ShaderDefs.push_back(MiscShadersNight_modeShaderDef()
.Param("scale_type", "source"));
	}
};
}
