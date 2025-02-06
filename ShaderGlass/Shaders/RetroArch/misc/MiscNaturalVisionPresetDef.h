/*
ShaderGlass preset misc / natural-vision imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/misc/natural-vision.slangp
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
