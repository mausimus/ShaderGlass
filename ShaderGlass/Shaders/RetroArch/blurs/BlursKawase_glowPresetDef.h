/*
ShaderGlass preset blurs / kawase_glow imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/blurs/kawase_glow.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class BlursKawase_glowPresetDef : public PresetDef
{
public:
	BlursKawase_glowPresetDef() : PresetDef{}
	{
		Name = "kawase_glow";
		Category = "blurs";
	}

	virtual void Build() {
         	ShaderDefs.push_back(StockStockShaderDef()
.Param("alias", "Reference")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(BlursShadersKawaseLinearizeShaderDef()
.Param("scale_type", "source")
.Param("srgb_framebuffer", "true"));
         	ShaderDefs.push_back(BlursShadersKawaseKawase0ShaderDef()
.Param("filter_linear", "true")
.Param("scale_type", "source")
.Param("srgb_framebuffer", "true")
.Param("wrap_mode", "mirrored_repeat"));
         	ShaderDefs.push_back(BlursShadersKawaseKawase1ShaderDef()
.Param("filter_linear", "true")
.Param("scale_type", "source")
.Param("srgb_framebuffer", "true")
.Param("wrap_mode", "mirrored_repeat"));
         	ShaderDefs.push_back(BlursShadersKawaseKawase1ShaderDef()
.Param("filter_linear", "true")
.Param("scale_type", "source")
.Param("srgb_framebuffer", "true")
.Param("wrap_mode", "mirrored_repeat"));
         	ShaderDefs.push_back(BlursShadersKawaseKawase2ShaderDef()
.Param("filter_linear", "true")
.Param("scale_type", "source")
.Param("srgb_framebuffer", "true")
.Param("wrap_mode", "mirrored_repeat"));
         	ShaderDefs.push_back(BlursShadersKawaseKawase3ShaderDef()
.Param("filter_linear", "true")
.Param("scale_type", "source")
.Param("srgb_framebuffer", "true")
.Param("wrap_mode", "mirrored_repeat"));
         	ShaderDefs.push_back(BlursShadersKawaseDelinearizeShaderDef()
.Param("filter_linear", "true"));
         	ShaderDefs.push_back(BlursShadersKawaseScreen_combineShaderDef());
	}
};
}
