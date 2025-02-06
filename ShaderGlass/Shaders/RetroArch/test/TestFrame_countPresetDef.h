/*
ShaderGlass preset test / frame_count imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/test/frame_count.slangp
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
