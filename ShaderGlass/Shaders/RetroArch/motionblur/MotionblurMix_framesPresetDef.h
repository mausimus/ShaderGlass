/*
ShaderGlass preset motionblur / mix_frames imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/motionblur/mix_frames.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class MotionblurMix_framesPresetDef : public PresetDef
{
public:
	MotionblurMix_framesPresetDef() : PresetDef{}
	{
		Name = "mix_frames";
		Category = "motionblur";
	}

	virtual void Build() {
         	ShaderDefs.push_back(MotionblurShadersMix_framesShaderDef()
.Param("filter_linear", "false"));
	}
};
}
