/*
ShaderGlass preset misc / natural-vision imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/6f921ee4815a7894a33855974285b04545a4fa42/misc/natural-vision.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class MiscNaturalVisionPresetDef : public PresetDef
{
public:
	MiscNaturalVisionPresetDef() : PresetDef{}
	{
		Name = "natural-vision";
		Category = "misc";
	}

	virtual void Build() {
         	ShaderDefs.push_back(MiscShadersNaturalVisionShaderDef()
.Param("scale_type", "source"));
	}
};
}
