/*
ShaderGlass preset motionblur / feedback imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/motionblur/feedback.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class MotionblurFeedbackPresetDef : public PresetDef
{
public:
	MotionblurFeedbackPresetDef() : PresetDef{}
	{
		Name = "feedback";
		Category = "motionblur";
	}

	virtual void Build() {
         	ShaderDefs.push_back(MotionblurShadersFeedbackShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
	}
};
}
