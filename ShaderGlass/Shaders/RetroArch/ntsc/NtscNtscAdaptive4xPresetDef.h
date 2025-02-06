/*
ShaderGlass preset ntsc / ntsc-adaptive-4x imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/ntsc/ntsc-adaptive-4x.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class NtscNtscAdaptive4xPresetDef : public PresetDef
{
public:
	NtscNtscAdaptive4xPresetDef() : PresetDef{}
	{
		Name = "ntsc-adaptive-4x";
		Category = "ntsc";
	}

	virtual void Build() {
         	ShaderDefs.push_back(NtscShadersNtscAdaptiveNtscPass1ShaderDef()
.Param("filter_linear", "false")
.Param("float_framebuffer", "true")
.Param("scale_type", "source")
.Param("scale_x", "1.0")
.Param("scale_y", "0.25"));
         	ShaderDefs.push_back(NtscShadersNtscAdaptiveNtscPass2ShaderDef()
.Param("filter_linear", "true")
.Param("scale_type", "source")
.Param("scale_x", "0.5")
.Param("scale_y", "4.0"));
	}
};
}
