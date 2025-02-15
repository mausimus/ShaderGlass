/*
ShaderGlass preset presets/handheld-plus-color-mod / retro-v3-nds-color imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/presets/handheld-plus-color-mod/retro-v3-nds-color.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class PresetsHandheldPlusColorModRetroV3NdsColorPresetDef : public PresetDef
{
public:
	PresetsHandheldPlusColorModRetroV3NdsColorPresetDef() : PresetDef{}
	{
		Name = "retro-v3-nds-color";
		Category = "presets/handheld-plus-color-mod";
	}

	virtual void Build() {
         	ShaderDefs.push_back(HandheldShadersColorNdsColorShaderDef()
.Param("filter_linear", "false"));
         	ShaderDefs.push_back(HandheldShadersRetroV3ShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "viewport"));
	}
};
}
