/*
ShaderGlass preset crt / phosphorlut imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/6f921ee4815a7894a33855974285b04545a4fa42/crt/phosphorlut.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class CrtPhosphorlutPresetDef : public PresetDef
{
public:
	CrtPhosphorlutPresetDef() : PresetDef{}
	{
		Name = "phosphorlut";
		Category = "crt";
	}

	virtual void Build() {
         	ShaderDefs.push_back(CrtShadersPhosphorlutScanlinesInterlaceLinearizeShaderDef()
.Param("alias", "firstPass")
.Param("filter_linear", "false")
.Param("scale", "2.0")
.Param("scale_type", "source")
.Param("srgb_framebuffer", "true"));
         	ShaderDefs.push_back(BlursShadersRoyaleBlur5fastVerticalShaderDef()
.Param("alias", "blurPassV")
.Param("filter_linear", "true")
.Param("scale", "1.0")
.Param("scale_type", "source")
.Param("srgb_framebuffer", "true"));
         	ShaderDefs.push_back(BlursShadersRoyaleBlur5fastHorizontalShaderDef()
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
.Param("filter_linear", "true")
.Param("scale_type", "viewport"));
            TextureDefs.push_back(CrtShadersPhosphorlutLutsShadowmaskTextureDef()
.Param("linear", "true")
.Param("name", "shadow")
.Param("wrap_mode", "repeat"));
            TextureDefs.push_back(CrtShadersPhosphorlutLutsApertureGrilleTextureDef()
.Param("linear", "true")
.Param("name", "aperture")
.Param("wrap_mode", "repeat"));
            TextureDefs.push_back(CrtShadersPhosphorlutLutsSlotmaskTextureDef()
.Param("linear", "true")
.Param("name", "slot")
.Param("wrap_mode", "repeat"));
	}
};
}
