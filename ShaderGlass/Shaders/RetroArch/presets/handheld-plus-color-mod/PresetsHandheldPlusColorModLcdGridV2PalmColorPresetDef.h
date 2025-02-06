/*
ShaderGlass preset presets-handheld-plus-color-mod / lcd-grid-v2-palm-color imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/presets/handheld-plus-color-mod/lcd-grid-v2-palm-color.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class PresetsHandheldPlusColorModLcdGridV2PalmColorPresetDef : public PresetDef
{
public:
	PresetsHandheldPlusColorModLcdGridV2PalmColorPresetDef() : PresetDef{}
	{
		Name = "lcd-grid-v2-palm-color";
		Category = "presets-handheld-plus-color-mod";
	}

	virtual void Build() {
         	ShaderDefs.push_back(HandheldShadersLcdCgwgLcdGridV2ShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "viewport"));
         	ShaderDefs.push_back(HandheldShadersColorPalmColorShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
	}
};
}
