/*
ShaderGlass preset blurs / kawase_blur_5pass imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/blurs/kawase_blur_5pass.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class BlursKawase_blur_5passPresetDef : public PresetDef
{
public:
	BlursKawase_blur_5passPresetDef() : PresetDef{}
	{
		Name = "kawase_blur_5pass";
		Category = "blurs";
	}

	virtual void Build() {
         	ShaderDefs.push_back(BlursKawaseLinearizeShaderDef()
.Param("scale_type", "source")
.Param("srgb_framebuffer", "true"));
         	ShaderDefs.push_back(BlursKawaseKawase0ShaderDef()
.Param("filter_linear", "true")
.Param("scale_type", "source")
.Param("srgb_framebuffer", "true"));
         	ShaderDefs.push_back(BlursKawaseKawase1ShaderDef()
.Param("filter_linear", "true")
.Param("scale_type", "source")
.Param("srgb_framebuffer", "true"));
         	ShaderDefs.push_back(BlursKawaseKawase1ShaderDef()
.Param("filter_linear", "true")
.Param("scale_type", "source")
.Param("srgb_framebuffer", "true"));
         	ShaderDefs.push_back(BlursKawaseKawase2ShaderDef()
.Param("filter_linear", "true")
.Param("scale_type", "source")
.Param("srgb_framebuffer", "true"));
         	ShaderDefs.push_back(BlursKawaseKawase3ShaderDef()
.Param("filter_linear", "true")
.Param("scale_type", "source")
.Param("srgb_framebuffer", "true"));
         	ShaderDefs.push_back(BlursKawaseDelinearizeShaderDef()
.Param("filter_linear", "true"));
	}
};
}
