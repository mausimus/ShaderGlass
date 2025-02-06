/*
ShaderGlass preset presets-handheld-plus-color-mod / retro-v2+image-adjustment imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/presets/handheld-plus-color-mod/retro-v2+image-adjustment.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class PresetsHandheldPlusColorModRetroV2ImageAdjustmentPresetDef : public PresetDef
{
public:
	PresetsHandheldPlusColorModRetroV2ImageAdjustmentPresetDef() : PresetDef{}
	{
		Name = "retro-v2+image-adjustment";
		Category = "presets-handheld-plus-color-mod";
	}

	virtual void Build() {
         	ShaderDefs.push_back(MiscShadersImageAdjustmentShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.000000")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(HandheldShadersRetroV2ShaderDef()
.Param("filter_linear", "false"));
	}
};
}
