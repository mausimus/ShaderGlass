/*
ShaderGlass preset misc / ss-gamma-ramp imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/misc/ss-gamma-ramp.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class MiscSsGammaRampPresetDef : public PresetDef
{
public:
	MiscSsGammaRampPresetDef() : PresetDef{}
	{
		Name = "ss-gamma-ramp";
		Category = "misc";
	}

	virtual void Build() {
         	ShaderDefs.push_back(MiscShadersSsGammaRampShaderDef()
.Param("scale_type", "source"));
	}
};
}
