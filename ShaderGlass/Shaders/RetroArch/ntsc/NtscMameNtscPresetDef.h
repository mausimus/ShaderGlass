/*
ShaderGlass preset ntsc / mame-ntsc imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/ntsc/mame-ntsc.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class NtscMameNtscPresetDef : public PresetDef
{
public:
	NtscMameNtscPresetDef() : PresetDef{}
	{
		Name = "mame-ntsc";
		Category = "ntsc";
	}

	virtual void Build() {
         	ShaderDefs.push_back(NtscShadersMameNtscMameNtscShaderDef()
.Param("filter_linear", "true")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(NtscShadersMameNtscMamePostprocShaderDef()
.Param("filter_linear", "true")
.Param("scale_type", "viewport"));
	}
};
}
