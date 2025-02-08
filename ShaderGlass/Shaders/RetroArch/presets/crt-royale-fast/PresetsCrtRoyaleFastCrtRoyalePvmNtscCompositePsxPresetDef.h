/*
ShaderGlass preset presets-crt-royale-fast / crt-royale-pvm-ntsc-composite-psx imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/presets/crt-royale-fast/crt-royale-pvm-ntsc-composite-psx.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class PresetsCrtRoyaleFastCrtRoyalePvmNtscCompositePsxPresetDef : public PresetDef
{
public:
	PresetsCrtRoyaleFastCrtRoyalePvmNtscCompositePsxPresetDef() : PresetDef{}
	{
		Name = "crt-royale-pvm-ntsc-composite-psx";
		Category = "presets-crt-royale-fast";
	}

	virtual void Build() {
         	ShaderDefs.push_back(StockStockShaderDef()
.Param("alias", "PrePass0"));
         	ShaderDefs.push_back(CrtShadersGuestAdvancedNtscNtscPass1ShaderDef()
.Param("alias", "NPass1")
.Param("filter_linear", "false")
.Param("float_framebuffer", "true")
.Param("scale_type_x", "source")
.Param("scale_type_y", "source")
.Param("scale_x", "4.0")
.Param("scale_y", "1.0"));
         	ShaderDefs.push_back(CrtShadersGuestAdvancedNtscNtscPass2ShaderDef()
.Param("filter_linear", "true")
.Param("float_framebuffer", "true")
.Param("scale_type", "source")
.Param("scale_x", "0.5")
.Param("scale_y", "1.0"));
         	ShaderDefs.push_back(CrtShadersGuestAdvancedNtscNtscPass3ShaderDef()
.Param("filter_linear", "true")
.Param("scale_type", "source")
.Param("scale_x", "1.0")
.Param("scale_y", "1.0"));
         	ShaderDefs.push_back(CrtShadersCrtRoyaleSrcFastCrtRoyaleFirstPassLinearizeCrtGammaBobFieldsShaderDef()
.Param("alias", "ORIG_LINEARIZED")
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source")
.Param("srgb_framebuffer", "true"));
         	ShaderDefs.push_back(CrtShadersCrtRoyaleSrcFastCrtRoyaleScanlinesVerticalInterlacingShaderDef()
.Param("alias", "VERTICAL_SCANLINES")
.Param("filter_linear", "true")
.Param("scale_type_x", "source")
.Param("scale_type_y", "viewport")
.Param("scale_x", "1.0")
.Param("scale_y", "1.0")
.Param("srgb_framebuffer", "true"));
         	ShaderDefs.push_back(CrtShadersCrtRoyaleSrcFastCrtRoyaleMaskResizeVerticalShaderDef()
.Param("filter_linear", "true")
.Param("scale_type_x", "absolute")
.Param("scale_type_y", "viewport")
.Param("scale_x", "64")
.Param("scale_y", "0.0625"));
         	ShaderDefs.push_back(CrtShadersCrtRoyaleSrcFastCrtRoyaleMaskResizeHorizontalShaderDef()
.Param("alias", "MASK_RESIZE")
.Param("filter_linear", "false")
.Param("scale_type_x", "viewport")
.Param("scale_type_y", "source")
.Param("scale_x", "0.0625")
.Param("scale_y", "1.0"));
         	ShaderDefs.push_back(CrtShadersCrtRoyaleSrcFastCrtRoyaleScanlinesHorizontalApplyMaskShaderDef()
.Param("alias", "MASKED_SCANLINES")
.Param("filter_linear", "true")
.Param("scale", "1.0")
.Param("scale_type", "viewport")
.Param("srgb_framebuffer", "true"));
         	ShaderDefs.push_back(CrtShadersCrtRoyaleSrcFastCrtRoyaleBrightpassShaderDef()
.Param("alias", "BRIGHTPASS")
.Param("filter_linear", "true")
.Param("scale", "1.0")
.Param("scale_type", "viewport")
.Param("srgb_framebuffer", "true"));
         	ShaderDefs.push_back(CrtShadersCrtRoyaleSrcFastCrtRoyaleBloomVerticalShaderDef()
.Param("filter_linear", "true")
.Param("scale", "1.0")
.Param("scale_type", "source")
.Param("srgb_framebuffer", "true"));
         	ShaderDefs.push_back(CrtShadersCrtRoyaleSrcFastCrtRoyaleBloomHorizontalReconstituteShaderDef()
.Param("filter_linear", "true")
.Param("scale", "1.0")
.Param("scale_type", "source")
.Param("srgb_framebuffer", "true")
.Param("wrap_mode", "clamp_to_edge"));
            TextureDefs.push_back(CrtShadersCrtRoyaleTileableLinearApertureGrille15Wide8And5d5SpacingResizeTo64BGRTextureDef()
.Param("linear", "true")
.Param("mipmap", "false")
.Param("name", "mask_grille_texture_small")
.Param("wrap_mode", "repeat"));
            TextureDefs.push_back(CrtShadersCrtRoyaleTileableLinearSlotMaskTall15Wide9And4d5Horizontal9d14VerticalSpacingResizeTo64BGRshiftedTextureDef()
.Param("linear", "true")
.Param("mipmap", "false")
.Param("name", "mask_slot_texture_small")
.Param("wrap_mode", "repeat"));
            TextureDefs.push_back(CrtShadersCrtRoyaleTileableLinearShadowMaskEDPResizeTo64TextureDef()
.Param("linear", "true")
.Param("mipmap", "false")
.Param("name", "mask_shadow_texture_small")
.Param("wrap_mode", "repeat"));
            OverrideParam("beam_horiz_filter", (float)3.000000);
            OverrideParam("cust_artifacting", (float)0.500000);
            OverrideParam("cust_fringing", (float)0.000000);
            OverrideParam("mask_triad_size_desired", (float)1.000000);
            OverrideParam("mask_type", (float)0.000000);
            OverrideParam("ntsc_cscale", (float)4.000000);
            OverrideParam("ntsc_cscale1", (float)2.250000);
            OverrideParam("ntsc_shape", (float)0.750000);
            OverrideParam("ntsc_sharp", (float)3.500000);
            OverrideParam("ntsc_taps", (float)24.000000);
	}
};
}
