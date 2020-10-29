/*
ShaderGlass preset ntsc / ntsc-256px-svideo-gauss-scanline imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/ntsc/ntsc-256px-svideo-gauss-scanline.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class NtscNtsc256pxSvideoGaussScanlinePresetDef : public PresetDef
{
public:
	NtscNtsc256pxSvideoGaussScanlinePresetDef() : PresetDef{}
	{
		Name = "ntsc-256px-svideo-gauss-scanline";
		Category = "ntsc";
	}

	virtual void Build() {
         	ShaderDefs.push_back(NtscShadersNtscPass1Svideo3phaseShaderDef()
.Param("filter_linear", "false")
.Param("float_framebuffer", "true")
.Param("frame_count_mod", "2")
.Param("scale_type_x", "absolute")
.Param("scale_type_y", "source")
.Param("scale_x", "1024")
.Param("scale_y", "1.0"));
         	ShaderDefs.push_back(NtscShadersNtscPass23phaseShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "source")
.Param("scale_x", "0.5")
.Param("scale_y", "1.0"));
         	ShaderDefs.push_back(NtscShadersNtscGaussPassShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type_x", "source")
.Param("scale_type_y", "viewport"));
         	ShaderDefs.push_back(NtscShadersNtscStockShaderDef()
.Param("filter_linear", "true"));
	}
};
}
