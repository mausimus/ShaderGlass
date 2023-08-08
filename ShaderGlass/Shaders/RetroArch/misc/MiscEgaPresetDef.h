/*
ShaderGlass preset misc / ega imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/6f921ee4815a7894a33855974285b04545a4fa42/misc/ega.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class MiscEgaPresetDef : public PresetDef
{
public:
	MiscEgaPresetDef() : PresetDef{}
	{
		Name = "ega";
		Category = "misc";
	}

	virtual void Build() {
         	ShaderDefs.push_back(MiscShadersEgaShaderDef()
.Param("scale_type", "source"));
	}
};
}
