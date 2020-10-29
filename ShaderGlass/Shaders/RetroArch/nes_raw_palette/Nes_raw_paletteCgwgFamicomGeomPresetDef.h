/*
ShaderGlass preset nes_raw_palette / cgwg-famicom-geom imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/nes_raw_palette/cgwg-famicom-geom.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class Nes_raw_paletteCgwgFamicomGeomPresetDef : public PresetDef
{
public:
	Nes_raw_paletteCgwgFamicomGeomPresetDef() : PresetDef{}
	{
		Name = "cgwg-famicom-geom";
		Category = "nes_raw_palette";
	}

	virtual void Build() {
         	ShaderDefs.push_back(Nes_raw_paletteShadersCgwgFamicomGeomCompositeEncodeShaderDef()
.Param("alias", "encode_pass")
.Param("filter_linear", "false")
.Param("float_framebuffer", "true")
.Param("frame_count_mod", "2")
.Param("scale_type", "source")
.Param("scale_x", "2.0"));
         	ShaderDefs.push_back(Nes_raw_paletteShadersCgwgFamicomGeomLowpassNotchDecodeYiqShaderDef()
.Param("filter_linear", "false")
.Param("float_framebuffer", "true")
.Param("frame_count_mod", "2"));
         	ShaderDefs.push_back(Nes_raw_paletteShadersCgwgFamicomGeomAdaptiveCombDecodeShaderDef()
.Param("filter_linear", "false")
.Param("float_framebuffer", "true")
.Param("frame_count_mod", "2"));
         	ShaderDefs.push_back(Nes_raw_paletteShadersCgwgFamicomGeomLowpassChromaShaderDef()
.Param("filter_linear", "false")
.Param("float_framebuffer", "true"));
         	ShaderDefs.push_back(Nes_raw_paletteShadersCgwgFamicomGeomCrtGeomFamicomShaderDef());
	}
};
}
