/*
ShaderGlass preset ntsc / ntsc-adaptive-tate imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/6f921ee4815a7894a33855974285b04545a4fa42/ntsc/ntsc-adaptive-tate.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class NtscNtscAdaptiveTatePresetDef : public PresetDef
{
public:
	NtscNtscAdaptiveTatePresetDef() : PresetDef{}
	{
		Name = "ntsc-adaptive-tate";
		Category = "ntsc";
	}

	virtual void Build() {
         	ShaderDefs.push_back(NtscShadersNtscAdaptiveTateNtscTatePass1ShaderDef()
.Param("filter_linear", "false")
.Param("float_framebuffer", "true")
.Param("scale_type", "source")
.Param("scale_x", "1.0")
.Param("scale_y", "4.0"));
         	ShaderDefs.push_back(NtscShadersNtscAdaptiveTateNtscTatePass2ShaderDef()
.Param("filter_linear", "true")
.Param("scale_type", "source")
.Param("scale_x", "1.0")
.Param("scale_y", "0.5"));
	}
};
}
