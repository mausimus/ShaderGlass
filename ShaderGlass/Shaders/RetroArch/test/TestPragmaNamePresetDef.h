/*
ShaderGlass preset test / pragma-name imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/test/pragma-name.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class TestPragmaNamePresetDef : public PresetDef
{
public:
	TestPragmaNamePresetDef() : PresetDef{}
	{
		Name = "pragma-name";
		Category = "test";
	}

	virtual void Build() {
         	ShaderDefs.push_back(TestPragmaNameShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
	}
};
}
