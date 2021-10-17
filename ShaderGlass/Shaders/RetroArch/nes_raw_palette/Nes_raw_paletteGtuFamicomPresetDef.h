/*
ShaderGlass preset nes_raw_palette / gtu-famicom imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/nes_raw_palette/gtu-famicom.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class Nes_raw_paletteGtuFamicomPresetDef : public PresetDef
{
public:
	Nes_raw_paletteGtuFamicomPresetDef() : PresetDef{}
	{
		Name = "gtu-famicom";
		Category = "nes_raw_palette";
	}

	virtual void Build() {
         	ShaderDefs.push_back(Nes_raw_paletteShadersGtuFamicomDAC_LUTShaderDef()
.Param("filter_linear", "false")
.Param("float_framebuffer", "true")
.Param("frame_count_mod", "2")
.Param("scale_type_x", "source")
.Param("scale_type_y", "source")
.Param("scale_x", "8.0")
.Param("scale_y", "1.0"));
         	ShaderDefs.push_back(Nes_raw_paletteShadersGtuFamicomLowPassShaderDef()
.Param("filter_linear", "false")
.Param("float_framebuffer", "true")
.Param("frame_count_mod", "32")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(Nes_raw_paletteShadersGtuFamicomCombFilterShaderDef()
.Param("filter_linear", "false")
.Param("float_framebuffer", "true")
.Param("frame_count_mod", "2")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(Nes_raw_paletteShadersGtuFamicomScaleXShaderDef()
.Param("filter_linear", "false")
.Param("float_framebuffer", "true")
.Param("scale_type_x", "viewport")
.Param("scale_type_y", "source")
.Param("scale_x", "1.0")
.Param("scale_y", "1.0"));
         	ShaderDefs.push_back(Nes_raw_paletteShadersGtuFamicomScaleYShaderDef()
.Param("filter_linear", "false")
.Param("float_framebuffer", "true")
.Param("scale_type_x", "source")
.Param("scale_type_y", "viewport")
.Param("scale_x", "1.0")
.Param("scale_y", "1.0"));
            TextureDefs.push_back(Nes_raw_paletteShadersGtuFamicomNesTableTextureDef()
.Param("linear", "false")
.Param("mipmap", "false")
.Param("name", "nestable")
.Param("wrap_mode", "clamp_to_border"));
	}
};
}
