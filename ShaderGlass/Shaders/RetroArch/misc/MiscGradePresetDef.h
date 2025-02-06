/*
ShaderGlass preset misc / grade imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/misc/grade.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class MiscGradePresetDef : public PresetDef
{
public:
	MiscGradePresetDef() : PresetDef{}
	{
		Name = "grade";
		Category = "misc";
	}

	virtual void Build() {
         	ShaderDefs.push_back(MiscShadersGradeShaderDef()
.Param("scale_type", "source"));
            TextureDefs.push_back(ReshadeShadersLUT32TextureDef()
.Param("linear", "true")
.Param("mipmap", "false")
.Param("name", "SamplerLUT1")
.Param("wrap_mode", "clamp_to_border"));
            TextureDefs.push_back(ReshadeShadersLUT64TextureDef()
.Param("linear", "true")
.Param("mipmap", "false")
.Param("name", "SamplerLUT2")
.Param("wrap_mode", "clamp_to_border"));
	}
};
}
