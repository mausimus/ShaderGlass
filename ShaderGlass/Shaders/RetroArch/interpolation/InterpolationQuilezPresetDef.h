/*
ShaderGlass preset interpolation / quilez imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/interpolation/quilez.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class InterpolationQuilezPresetDef : public PresetDef
{
public:
	InterpolationQuilezPresetDef() : PresetDef{}
	{
		Name = "quilez";
		Category = "interpolation";
	}

	virtual void Build() {
         	ShaderDefs.push_back(InterpolationShadersQuilezShaderDef()
.Param("filter_linear", "true")
.Param("scale_type", "viewport"));
	}
};
}
