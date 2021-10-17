/*
ShaderGlass preset presets / ntsc-phosphorlut imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/presets/ntsc-phosphorlut.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class PresetsNtscPhosphorlutPresetDef : public PresetDef
{
public:
	PresetsNtscPhosphorlutPresetDef() : PresetDef{}
	{
		Name = "ntsc-phosphorlut";
		Category = "presets";
	}

	virtual void Build() {
         	ShaderDefs.push_back(NtscShadersNtscAdaptiveNtscPass1ShaderDef()
.Param("filter_linear", "false")
.Param("float_framebuffer", "true")
.Param("frame_count_mod", "2")
.Param("scale_type", "source")
.Param("scale_x", "4.0")
.Param("scale_y", "1.0"));
         	ShaderDefs.push_back(NtscShadersNtscAdaptiveNtscPass2ShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "source")
.Param("scale_x", "0.5")
.Param("scale_y", "1.0"));
         	ShaderDefs.push_back(CrtShadersPhosphorlutScanlinesInterlaceLinearizeShaderDef()
.Param("alias", "firstPass")
.Param("filter_linear", "false")
.Param("scale", "2.0")
.Param("scale_type", "source")
.Param("srgb_framebuffer", "true"));
         	ShaderDefs.push_back(BlursBlur5fastVerticalShaderDef()
.Param("alias", "blurPassV")
.Param("filter_linear", "true")
.Param("scale", "1.0")
.Param("scale_type", "source")
.Param("srgb_framebuffer", "true"));
         	ShaderDefs.push_back(BlursBlur5fastHorizontalShaderDef()
.Param("alias", "blurPass")
.Param("filter_linear", "true")
.Param("scale", "1.0")
.Param("scale_type", "source")
.Param("srgb_framebuffer", "true"));
         	ShaderDefs.push_back(CrtShadersPhosphorlutPhosphorlutPass0ShaderDef()
.Param("alias", "phosphorPass")
.Param("filter_linear", "true")
.Param("scale_type", "source")
.Param("scale_x", "4.0")
.Param("scale_y", "2.0")
.Param("srgb_framebuffer", "true"));
         	ShaderDefs.push_back(CrtShadersPhosphorlutPhosphorlutPass1ShaderDef()
.Param("filter_linear", "true"));
            TextureDefs.push_back(CrtShadersPhosphorlutLutsShadowmaskTextureDef()
.Param("name", "shadow"));
            TextureDefs.push_back(CrtShadersPhosphorlutLutsApertureGrilleTextureDef()
.Param("name", "aperture"));
            TextureDefs.push_back(CrtShadersPhosphorlutLutsSlotmaskTextureDef()
.Param("name", "slot"));
	}
};
}
