/*
ShaderGlass preset ntsc / ntsc-md-rainbows imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/6f921ee4815a7894a33855974285b04545a4fa42/ntsc/ntsc-md-rainbows.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class NtscNtscMdRainbowsPresetDef : public PresetDef
{
public:
	NtscNtscMdRainbowsPresetDef() : PresetDef{}
	{
		Name = "ntsc-md-rainbows";
		Category = "ntsc";
	}

	virtual void Build() {
         	ShaderDefs.push_back(CrtShadersMame_hlslShadersMame_ntsc_encodeShaderDef()
.Param("filter_linear", "true")
.Param("scale", "1.000000")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(CrtShadersMame_hlslShadersMame_ntsc_decodeShaderDef()
.Param("filter_linear", "true")
.Param("scale_type", "source"));
	}
};
}
