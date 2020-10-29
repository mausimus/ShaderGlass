/*
ShaderGlass preset test / feedback-noncausal imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/test/feedback-noncausal.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class TestFeedbackNoncausalPresetDef : public PresetDef
{
public:
	TestFeedbackNoncausalPresetDef() : PresetDef{}
	{
		Name = "feedback-noncausal";
		Category = "test";
	}

	virtual void Build() {
         	ShaderDefs.push_back(TestFeedbackNoncausalShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
	}
};
}
