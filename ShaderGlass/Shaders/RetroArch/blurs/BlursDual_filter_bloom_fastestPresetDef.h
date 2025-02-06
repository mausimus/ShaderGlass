/*
ShaderGlass preset blurs / dual_filter_bloom_fastest imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/blurs/dual_filter_bloom_fastest.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class BlursDual_filter_bloom_fastestPresetDef : public PresetDef
{
public:
	BlursDual_filter_bloom_fastestPresetDef() : PresetDef{}
	{
		Name = "dual_filter_bloom_fastest";
		Category = "blurs";
	}

	virtual void Build() {
         	ShaderDefs.push_back(BlursShadersKawaseLinearizeShaderDef()
.Param("alias", "Input")
.Param("float_framebuffer", "true")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(BlursShadersDual_filterDownsample_bloomShaderDef()
.Param("alias", "Downsample1")
.Param("filter_linear", "false")
.Param("float_framebuffer", "true")
.Param("scale", "0.5")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(BlursShadersDual_filterDownsampleShaderDef()
.Param("alias", "Downsample2")
.Param("filter_linear", "true")
.Param("float_framebuffer", "true")
.Param("scale", "0.5")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(BlursShadersDual_filterUpsampleShaderDef()
.Param("alias", "Upsample")
.Param("filter_linear", "true")
.Param("float_framebuffer", "true")
.Param("scale", "2.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(BlursShadersDual_filterNaive_resampleShaderDef()
.Param("filter_linear", "true")
.Param("float_framebuffer", "true")
.Param("scale_type", "absolute")
.Param("scale_x", "64")
.Param("scale_y", "64"));
         	ShaderDefs.push_back(BlursShadersDual_filterMoving_avgShaderDef()
.Param("alias", "MovingAverage")
.Param("filter_linear", "true")
.Param("float_framebuffer", "true")
.Param("mipmap_input", "true")
.Param("scale_type", "absolute")
.Param("scale_x", "2")
.Param("scale_y", "2"));
         	ShaderDefs.push_back(BlursShadersDual_filterBloom_blend_fastestShaderDef()
.Param("filter_linear", "true")
.Param("float_framebuffer", "true")
.Param("scale", "1.0")
.Param("scale_type", "viewport"));
         	ShaderDefs.push_back(BlursShadersKawaseDelinearizeShaderDef()
.Param("filter_linear", "false")
.Param("float_framebuffer", "true")
.Param("scale", "1.0")
.Param("scale_type", "viewport"));
	}
};
}
