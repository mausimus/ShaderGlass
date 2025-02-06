/*
ShaderGlass preset motionblur / mix_frames_smart imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/motionblur/mix_frames_smart.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class MotionblurMix_frames_smartPresetDef : public PresetDef
{
public:
	MotionblurMix_frames_smartPresetDef() : PresetDef{}
	{
		Name = "mix_frames_smart";
		Category = "motionblur";
	}

	virtual void Build() {
         	ShaderDefs.push_back(MotionblurShadersMix_frames_smartShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "source"));
	}
};
}
