/*
ShaderGlass preset ntsc / ntsc-feather imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/6f921ee4815a7894a33855974285b04545a4fa42/ntsc/ntsc-feather.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class NtscNtscFeatherPresetDef : public PresetDef
{
public:
	NtscNtscFeatherPresetDef() : PresetDef{}
	{
		Name = "ntsc-feather";
		Category = "ntsc";
	}

	virtual void Build() {
         	ShaderDefs.push_back(NtscShadersNtscSimpleNtscFeatherShaderDef()
.Param("filter_linear", "true"));
	}
};
}
