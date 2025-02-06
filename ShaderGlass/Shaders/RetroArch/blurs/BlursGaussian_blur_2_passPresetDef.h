/*
ShaderGlass preset blurs / gaussian_blur_2_pass imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/blurs/gaussian_blur_2_pass.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class BlursGaussian_blur_2_passPresetDef : public PresetDef
{
public:
	BlursGaussian_blur_2_passPresetDef() : PresetDef{}
	{
		Name = "gaussian_blur_2_pass";
		Category = "blurs";
	}

	virtual void Build() {
         	ShaderDefs.push_back(BlursShadersGaussian_blur_filteringGaussian_horizontalShaderDef()
.Param("filter_linear", "false")
.Param("scale_type_x", "viewport")
.Param("scale_type_y", "source")
.Param("scale_x", "1.0")
.Param("scale_y", "1.0"));
         	ShaderDefs.push_back(BlursShadersGaussian_blur_filteringGaussian_verticalShaderDef()
.Param("filter_linear", "false")
.Param("scale_type_x", "viewport")
.Param("scale_type_y", "viewport")
.Param("scale_x", "1.0")
.Param("scale_y", "1.0"));
	}
};
}
