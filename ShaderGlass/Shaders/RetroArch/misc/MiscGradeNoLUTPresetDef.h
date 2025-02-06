/*
ShaderGlass preset misc / grade-no-LUT imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/misc/grade-no-LUT.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class MiscGradeNoLUTPresetDef : public PresetDef
{
public:
	MiscGradeNoLUTPresetDef() : PresetDef{}
	{
		Name = "grade-no-LUT";
		Category = "misc";
	}

	virtual void Build() {
         	ShaderDefs.push_back(MiscShadersGradeNoLUTShaderDef()
.Param("scale_type", "viewport"));
	}
};
}
