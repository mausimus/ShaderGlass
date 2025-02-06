/*
ShaderGlass preset interpolation / catmull-rom imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/interpolation/catmull-rom.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class InterpolationCatmullRomPresetDef : public PresetDef
{
public:
	InterpolationCatmullRomPresetDef() : PresetDef{}
	{
		Name = "catmull-rom";
		Category = "interpolation";
	}

	virtual void Build() {
         	ShaderDefs.push_back(InterpolationShadersCatmullRomShaderDef()
.Param("filter_linear", "true")
.Param("scale_type", "viewport"));
	}
};
}
