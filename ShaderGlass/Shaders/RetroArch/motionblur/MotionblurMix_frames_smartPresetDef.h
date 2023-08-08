/*
ShaderGlass preset motionblur / mix_frames_smart imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/6f921ee4815a7894a33855974285b04545a4fa42/motionblur/mix_frames_smart.slangp
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
