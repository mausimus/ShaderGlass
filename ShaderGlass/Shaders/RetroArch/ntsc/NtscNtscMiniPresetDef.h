/*
ShaderGlass preset ntsc / ntsc-mini imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/ntsc/ntsc-mini.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class NtscNtscMiniPresetDef : public PresetDef
{
public:
	NtscNtscMiniPresetDef() : PresetDef{}
	{
		Name = "ntsc-mini";
		Category = "ntsc";
	}

	virtual void Build() {
         	ShaderDefs.push_back(NtscShadersNtscSimpleNtscMiniShaderDef()
.Param("filter_linear", "false")
.Param("scale_type_x", "source")
.Param("scale_type_y", "source")
.Param("scale_x", "1.000000")
.Param("scale_y", "1.000000"));
            OverrideParam("feedback_pass", (float)0.000000);
	}
};
}
