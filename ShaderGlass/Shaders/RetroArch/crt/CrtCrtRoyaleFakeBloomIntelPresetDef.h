/*
ShaderGlass preset crt / crt-royale-fake-bloom-intel imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/crt/crt-royale-fake-bloom-intel.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class CrtCrtRoyaleFakeBloomIntelPresetDef : public PresetDef
{
public:
	CrtCrtRoyaleFakeBloomIntelPresetDef() : PresetDef{}
	{
		Name = "crt-royale-fake-bloom-intel";
		Category = "crt";
	}

	virtual void Build() {
         	ShaderDefs.push_back(CrtShadersCrtRoyaleSrcCrtRoyaleFirstPassLinearizeCrtGammaBobFieldsShaderDef()
.Param("alias", "ORIG_LINEARIZED")
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source")
.Param("srgb_framebuffer", "true"));
         	ShaderDefs.push_back(CrtShadersCrtRoyaleSrcCrtRoyaleScanlinesVerticalInterlacingShaderDef()
.Param("alias", "VERTICAL_SCANLINES")
.Param("filter_linear", "true")
.Param("scale_type_x", "source")
.Param("scale_type_y", "viewport")
.Param("scale_x", "1.0")
.Param("scale_y", "1.0")
.Param("srgb_framebuffer", "true"));
         	ShaderDefs.push_back(CrtShadersCrtRoyaleSrcCrtRoyaleBloomApproxFakeBloomIntelShaderDef()
.Param("alias", "BLOOM_APPROX")
.Param("filter_linear", "true")
.Param("scale_type", "absolute")
.Param("scale_x", "400")
.Param("scale_y", "300")
.Param("srgb_framebuffer", "true"));
         	ShaderDefs.push_back(BlursShadersRoyaleBlur9fastVerticalShaderDef()
.Param("filter_linear", "true")
.Param("scale", "1.0")
.Param("scale_type", "source")
.Param("srgb_framebuffer", "true"));
         	ShaderDefs.push_back(BlursShadersRoyaleBlur9fastHorizontalShaderDef()
.Param("alias", "HALATION_BLUR")
.Param("filter_linear", "true")
.Param("scale", "1.0")
.Param("scale_type", "source")
.Param("srgb_framebuffer", "true"));
         	ShaderDefs.push_back(CrtShadersCrtRoyaleSrcCrtRoyaleScanlinesHorizontalApplyMaskFakeBloomIntelShaderDef()
.Param("alias", "MASKED_SCANLINES")
.Param("filter_linear", "true")
.Param("scale", "1.0")
.Param("scale_type", "viewport")
.Param("srgb_framebuffer", "true"));
         	ShaderDefs.push_back(CrtShadersCrtRoyaleSrcCrtRoyaleGeometryAaLastPassIntelShaderDef()
.Param("filter_linear", "true")
.Param("mipmap_input", "true")
.Param("scale_type", "viewport")
.Param("wrap_mode", "clamp_to_edge"));
            TextureDefs.push_back(CrtShadersCrtRoyaleTileableLinearApertureGrille15Wide8And5d5SpacingResizeTo64TextureDef()
.Param("linear", "true")
.Param("mipmap", "false")
.Param("name", "mask_grille_texture_small")
.Param("wrap_mode", "repeat"));
            TextureDefs.push_back(CrtShadersCrtRoyaleTileableLinearApertureGrille15Wide8And5d5SpacingTextureDef()
.Param("linear", "true")
.Param("mipmap", "true")
.Param("name", "mask_grille_texture_large")
.Param("wrap_mode", "repeat"));
            TextureDefs.push_back(CrtShadersCrtRoyaleTileableLinearSlotMaskTall15Wide9And4d5Horizontal9d14VerticalSpacingResizeTo64TextureDef()
.Param("linear", "true")
.Param("mipmap", "false")
.Param("name", "mask_slot_texture_small")
.Param("wrap_mode", "repeat"));
            TextureDefs.push_back(CrtShadersCrtRoyaleTileableLinearSlotMaskTall15Wide9And4d5Horizontal9d14VerticalSpacingTextureDef()
.Param("linear", "true")
.Param("mipmap", "true")
.Param("name", "mask_slot_texture_large")
.Param("wrap_mode", "repeat"));
            TextureDefs.push_back(CrtShadersCrtRoyaleTileableLinearShadowMaskEDPResizeTo64TextureDef()
.Param("linear", "true")
.Param("mipmap", "false")
.Param("name", "mask_shadow_texture_small")
.Param("wrap_mode", "repeat"));
            TextureDefs.push_back(CrtShadersCrtRoyaleTileableLinearShadowMaskEDPTextureDef()
.Param("linear", "true")
.Param("mipmap", "true")
.Param("name", "mask_shadow_texture_large")
.Param("wrap_mode", "repeat"));
	}
};
}
