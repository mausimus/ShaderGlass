/*
ShaderGlass preset interpolation / bicubic-5-taps imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/interpolation/bicubic-5-taps.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class InterpolationBicubic5TapsPresetDef : public PresetDef
{
public:
	InterpolationBicubic5TapsPresetDef() : PresetDef{}
	{
		Name = "bicubic-5-taps";
		Category = "interpolation";
	}

	virtual void Build() {
         	ShaderDefs.push_back(InterpolationShadersBicubic5TapsShaderDef()
.Param("filter_linear", "true")
.Param("scale_type", "viewport"));
	}
};
}
