/*
ShaderGlass preset border / blur_fill imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/border/blur_fill.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class BorderBlur_fillPresetDef : public PresetDef
{
public:
	BorderBlur_fillPresetDef() : PresetDef{}
	{
		Name = "blur_fill";
		Category = "border";
	}

	virtual void Build() {
         	ShaderDefs.push_back(BlursShadersKawaseLinearizeShaderDef()
.Param("alias", "Input")
.Param("float_framebuffer", "true")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(BlursShadersGauss_4tapGauss_2tap_hShaderDef()
.Param("filter_linear", "true")
.Param("float_framebuffer", "true")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(BlursShadersGauss_4tapGauss_2tap_vShaderDef()
.Param("filter_linear", "true")
.Param("float_framebuffer", "true")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(BorderShadersBlur_fillRender_sampling_areasShaderDef()
.Param("alias", "Tiled")
.Param("filter_linear", "true")
.Param("float_framebuffer", "true")
.Param("scale_type", "source")
.Param("scale_x", "1.1")
.Param("scale_y", "1.1"));
         	ShaderDefs.push_back(BlursShadersDual_filterDownsampleShaderDef()
.Param("filter_linear", "true")
.Param("float_framebuffer", "true")
.Param("scale", "0.5")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(BlursShadersDual_filterDownsampleShaderDef()
.Param("filter_linear", "true")
.Param("float_framebuffer", "true")
.Param("scale", "0.5")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(BlursShadersDual_filterDownsampleShaderDef()
.Param("filter_linear", "true")
.Param("float_framebuffer", "true")
.Param("scale", "0.5")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(BlursShadersDual_filterUpsampleShaderDef()
.Param("filter_linear", "true")
.Param("float_framebuffer", "true")
.Param("scale", "2.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(BlursShadersDual_filterUpsampleShaderDef()
.Param("filter_linear", "true")
.Param("float_framebuffer", "true")
.Param("scale", "2.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(BlursShadersDual_filterUpsampleShaderDef()
.Param("alias", "Blurred")
.Param("filter_linear", "true")
.Param("float_framebuffer", "true")
.Param("scale", "2.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(BorderShadersBlur_fillComposeShaderDef()
.Param("filter_linear", "true")
.Param("float_framebuffer", "true")
.Param("scale", "1.0")
.Param("scale_type", "viewport"));
         	ShaderDefs.push_back(BlursShadersKawaseDelinearizeShaderDef()
.Param("filter_linear", "true")
.Param("float_framebuffer", "true")
.Param("scale", "1.0")
.Param("scale_type", "viewport"));
	}
};
}
