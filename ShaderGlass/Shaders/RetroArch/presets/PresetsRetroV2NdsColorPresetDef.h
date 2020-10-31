/*
ShaderGlass preset presets / retro-v2+nds-color imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/presets/retro-v2+nds-color.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class PresetsRetroV2NdsColorPresetDef : public PresetDef
{
public:
	PresetsRetroV2NdsColorPresetDef() : PresetDef{}
	{
		Name = "retro-v2+nds-color";
		Category = "presets";
	}

	virtual void Build() {
         	ShaderDefs.push_back(HandheldShadersColorNdsColorShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.000000")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(HandheldShadersRetroV2ShaderDef()
.Param("filter_linear", "false"));
	}
};
}