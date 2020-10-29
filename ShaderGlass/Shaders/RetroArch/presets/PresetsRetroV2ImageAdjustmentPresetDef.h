/*
ShaderGlass preset presets / retro-v2+image-adjustment imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/presets/retro-v2+image-adjustment.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class PresetsRetroV2ImageAdjustmentPresetDef : public PresetDef
{
public:
	PresetsRetroV2ImageAdjustmentPresetDef() : PresetDef{}
	{
		Name = "retro-v2+image-adjustment";
		Category = "presets";
	}

	virtual void Build() {
         	ShaderDefs.push_back(MiscImageAdjustmentShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.000000")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(HandheldShadersRetroV2ShaderDef()
.Param("filter_linear", "false"));
	}
};
}
