/*
ShaderGlass preset misc / chroma imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/6f921ee4815a7894a33855974285b04545a4fa42/misc/chroma.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class MiscChromaPresetDef : public PresetDef
{
public:
	MiscChromaPresetDef() : PresetDef{}
	{
		Name = "chroma";
		Category = "misc";
	}

	virtual void Build() {
         	ShaderDefs.push_back(MiscShadersChromaShaderDef()
.Param("scale_type", "source"));
	}
};
}
