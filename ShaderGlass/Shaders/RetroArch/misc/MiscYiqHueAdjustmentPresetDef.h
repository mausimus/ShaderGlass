/*
ShaderGlass preset misc / yiq-hue-adjustment imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/misc/yiq-hue-adjustment.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class MiscYiqHueAdjustmentPresetDef : public PresetDef
{
public:
	MiscYiqHueAdjustmentPresetDef() : PresetDef{}
	{
		Name = "yiq-hue-adjustment";
		Category = "misc";
	}

	virtual void Build() {
         	ShaderDefs.push_back(MiscShadersYiqHueAdjustmentShaderDef()
.Param("scale_type", "source"));
	}
};
}
