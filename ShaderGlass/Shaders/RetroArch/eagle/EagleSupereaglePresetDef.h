/*
ShaderGlass preset eagle / supereagle imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/eagle/supereagle.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class EagleSupereaglePresetDef : public PresetDef
{
public:
	EagleSupereaglePresetDef() : PresetDef{}
	{
		Name = "supereagle";
		Category = "eagle";
	}

	virtual void Build() {
         	ShaderDefs.push_back(EagleShadersSupereagleShaderDef()
.Param("filter_linear", "false"));
	}
};
}
