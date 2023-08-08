/*
ShaderGlass preset blurs / kawase_blur_9pass imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/6f921ee4815a7894a33855974285b04545a4fa42/blurs/kawase_blur_9pass.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class BlursKawase_blur_9passPresetDef : public PresetDef
{
public:
	BlursKawase_blur_9passPresetDef() : PresetDef{}
	{
		Name = "kawase_blur_9pass";
		Category = "blurs";
	}

	virtual void Build() {
         	ShaderDefs.push_back(BlursShadersKawaseLinearizeShaderDef()
.Param("scale_type", "source")
.Param("srgb_framebuffer", "true"));
         	ShaderDefs.push_back(BlursShadersKawaseKawase0ShaderDef()
.Param("filter_linear", "true")
.Param("scale_type", "source")
.Param("srgb_framebuffer", "true"));
         	ShaderDefs.push_back(BlursShadersKawaseKawase1ShaderDef()
.Param("filter_linear", "true")
.Param("scale_type", "source")
.Param("srgb_framebuffer", "true"));
         	ShaderDefs.push_back(BlursShadersKawaseKawase2ShaderDef()
.Param("filter_linear", "true")
.Param("scale_type", "source")
.Param("srgb_framebuffer", "true"));
         	ShaderDefs.push_back(BlursShadersKawaseKawase3ShaderDef()
.Param("filter_linear", "true")
.Param("scale_type", "source")
.Param("srgb_framebuffer", "true"));
         	ShaderDefs.push_back(BlursShadersKawaseKawase4ShaderDef()
.Param("filter_linear", "true")
.Param("scale_type", "source")
.Param("srgb_framebuffer", "true"));
         	ShaderDefs.push_back(BlursShadersKawaseKawase4ShaderDef()
.Param("filter_linear", "true")
.Param("scale_type", "source")
.Param("srgb_framebuffer", "true"));
         	ShaderDefs.push_back(BlursShadersKawaseKawase5ShaderDef()
.Param("filter_linear", "true")
.Param("scale_type", "source")
.Param("srgb_framebuffer", "true"));
         	ShaderDefs.push_back(BlursShadersKawaseKawase6ShaderDef()
.Param("filter_linear", "true")
.Param("scale_type", "source")
.Param("srgb_framebuffer", "true"));
         	ShaderDefs.push_back(BlursShadersKawaseKawase7ShaderDef()
.Param("filter_linear", "true")
.Param("scale_type", "source")
.Param("srgb_framebuffer", "true"));
         	ShaderDefs.push_back(BlursShadersKawaseDelinearizeShaderDef()
.Param("filter_linear", "true"));
	}
};
}
