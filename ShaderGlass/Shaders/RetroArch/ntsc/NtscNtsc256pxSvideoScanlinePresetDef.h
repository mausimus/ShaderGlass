/*
ShaderGlass preset ntsc / ntsc-256px-svideo-scanline imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/ntsc/ntsc-256px-svideo-scanline.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class NtscNtsc256pxSvideoScanlinePresetDef : public PresetDef
{
public:
	NtscNtsc256pxSvideoScanlinePresetDef() : PresetDef{}
	{
		Name = "ntsc-256px-svideo-scanline";
		Category = "ntsc";
	}

	virtual void Build() {
         	ShaderDefs.push_back(NtscShadersMaisterNtscPass1Svideo3phaseShaderDef()
.Param("filter_linear", "false")
.Param("float_framebuffer", "true")
.Param("frame_count_mod", "2")
.Param("scale_type_x", "absolute")
.Param("scale_type_y", "source")
.Param("scale_x", "1024")
.Param("scale_y", "1.0"));
         	ShaderDefs.push_back(NtscShadersMaisterNtscPass23phaseShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "source")
.Param("scale_x", "0.5")
.Param("scale_y", "1.0"));
         	ShaderDefs.push_back(NtscShadersMaisterNtscGaussPassShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type_x", "source")
.Param("scale_type_y", "viewport"));
         	ShaderDefs.push_back(NtscShadersMaisterNtscStockShaderDef()
.Param("filter_linear", "true"));
	}
};
}
