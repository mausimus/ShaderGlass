/*
ShaderGlass preset misc / night-mode imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/6f921ee4815a7894a33855974285b04545a4fa42/misc/night-mode.slangp
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
