/*
ShaderGlass preset misc / ss-gamma-ramp imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/6f921ee4815a7894a33855974285b04545a4fa42/misc/ss-gamma-ramp.slangp
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
