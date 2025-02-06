/*
ShaderGlass preset border / average_fill imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/border/average_fill.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class BorderAverage_fillPresetDef : public PresetDef
{
public:
	BorderAverage_fillPresetDef() : PresetDef{}
	{
		Name = "average_fill";
		Category = "border";
	}

	virtual void Build() {
         	ShaderDefs.push_back(BlursShadersKawaseLinearizeShaderDef()
.Param("alias", "Input")
.Param("float_framebuffer", "true")
.Param("scale_type", "source")
.Param("scale_x", "1.0")
.Param("scale_y", "1.0"));
         	ShaderDefs.push_back(BorderShadersAverage_fillCrop_and_sample_topShaderDef()
.Param("alias", "Top")
.Param("filter_linear", "true")
.Param("float_framebuffer", "true")
.Param("scale_type", "absolute")
.Param("scale_x", "512")
.Param("scale_y", "64"));
         	ShaderDefs.push_back(BorderShadersAverage_fillCrop_and_sample_bottomShaderDef()
.Param("alias", "Bottom")
.Param("filter_linear", "true")
.Param("float_framebuffer", "true")
.Param("mipmap_input", "true")
.Param("scale_type", "absolute")
.Param("scale_x", "512")
.Param("scale_y", "64"));
         	ShaderDefs.push_back(BorderShadersAverage_fillCrop_and_sample_leftShaderDef()
.Param("alias", "Left")
.Param("filter_linear", "true")
.Param("float_framebuffer", "true")
.Param("mipmap_input", "true")
.Param("scale_type", "absolute")
.Param("scale_x", "64")
.Param("scale_y", "512"));
         	ShaderDefs.push_back(BorderShadersAverage_fillCrop_and_sample_rightShaderDef()
.Param("alias", "Right")
.Param("filter_linear", "true")
.Param("float_framebuffer", "true")
.Param("mipmap_input", "true")
.Param("scale_type", "absolute")
.Param("scale_x", "64")
.Param("scale_y", "512"));
         	ShaderDefs.push_back(BorderShadersAverage_fillComposeShaderDef()
.Param("filter_linear", "true")
.Param("float_framebuffer", "true")
.Param("mipmap_input", "true")
.Param("scale_type", "viewport")
.Param("scale_x", "1.0")
.Param("scale_y", "1.0"));
         	ShaderDefs.push_back(BlursShadersKawaseDelinearizeShaderDef()
.Param("filter_linear", "true")
.Param("float_framebuffer", "true")
.Param("scale_type", "viewport")
.Param("scale_x", "1.0")
.Param("scale_y", "1.0"));
	}
};
}
