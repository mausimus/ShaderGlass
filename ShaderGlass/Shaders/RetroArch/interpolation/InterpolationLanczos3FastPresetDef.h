/*
ShaderGlass preset interpolation / lanczos3-fast imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/interpolation/lanczos3-fast.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class InterpolationLanczos3FastPresetDef : public PresetDef
{
public:
	InterpolationLanczos3FastPresetDef() : PresetDef{}
	{
		Name = "lanczos3-fast";
		Category = "interpolation";
	}

	virtual void Build() {
         	ShaderDefs.push_back(InterpolationShadersLanczos3XShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type_x", "viewport")
.Param("scale_type_y", "source"));
         	ShaderDefs.push_back(InterpolationShadersLanczos3YShaderDef()
.Param("filter_linear", "false"));
	}
};
}
