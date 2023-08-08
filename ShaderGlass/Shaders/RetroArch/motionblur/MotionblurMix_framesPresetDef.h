/*
ShaderGlass preset motionblur / mix_frames imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/6f921ee4815a7894a33855974285b04545a4fa42/motionblur/mix_frames.slangp
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
.Param("filter_linear", "false")
.Param("scale_type", "source"));
	}
};
}
