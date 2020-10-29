/*
ShaderGlass preset interpolation / pixellate imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/interpolation/pixellate.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class InterpolationPixellatePresetDef : public PresetDef
{
public:
	InterpolationPixellatePresetDef() : PresetDef{}
	{
		Name = "pixellate";
		Category = "interpolation";
	}

	virtual void Build() {
         	ShaderDefs.push_back(InterpolationShadersPixellateShaderDef()
.Param("filter_linear", "false"));
	}
};
}
