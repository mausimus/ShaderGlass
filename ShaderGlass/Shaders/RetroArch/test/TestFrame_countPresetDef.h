/*
ShaderGlass preset test / frame_count imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/test/frame_count.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class TestFrame_countPresetDef : public PresetDef
{
public:
	TestFrame_countPresetDef() : PresetDef{}
	{
		Name = "frame_count";
		Category = "test";
	}

	virtual void Build() {
         	ShaderDefs.push_back(TestFrame_countShaderDef()
.Param("filter_linear", "false")
.Param("frame_count_mod", "100")
.Param("scale", "1.0")
.Param("scale_type", "source"));
	}
};
}
