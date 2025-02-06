/*
ShaderGlass preset ntsc / ntsc-blastem imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/ntsc/ntsc-blastem.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class NtscNtscBlastemPresetDef : public PresetDef
{
public:
	NtscNtscBlastemPresetDef() : PresetDef{}
	{
		Name = "ntsc-blastem";
		Category = "ntsc";
	}

	virtual void Build() {
         	ShaderDefs.push_back(NtscShadersNtscBlastemShaderDef()
.Param("scale", "1.0")
.Param("scale_type", "source"));
	}
};
}
