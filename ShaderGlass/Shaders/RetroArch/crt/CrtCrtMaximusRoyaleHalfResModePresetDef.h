/*
ShaderGlass preset crt / crt-maximus-royale-half-res-mode imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/6f921ee4815a7894a33855974285b04545a4fa42/crt/crt-maximus-royale-half-res-mode.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class CrtCrtMaximusRoyaleHalfResModePresetDef : public PresetDef
{
public:
	CrtCrtMaximusRoyaleHalfResModePresetDef() : PresetDef{}
	{
		Name = "crt-maximus-royale-half-res-mode";
		Category = "crt";
	}

	virtual void Build() {
         	ShaderDefs.push_back(StockStockShaderDef()
.Param("alias", "maximus_refpass"));
         	ShaderDefs.push_back(CrtShadersCrtMaximusRoyaleSrcH_blurShaderDef()
.Param("filter_linear", "false")
.Param("scale", "0.5")
.Param("scale_type", "source")
.Param("srgb_framebuffer", "true"));
         	ShaderDefs.push_back(CrtShadersCrtRoyaleSrcCrtRoyaleFirstPassLinearizeCrtGammaBobFieldsShaderDef()
.Param("alias", "ORIG_LINEARIZED")
.Param("filter_linear", "true")
.Param("scale", "1.0")
.Param("scale_type", "source")
.Param("srgb_framebuffer", "true"));
         	ShaderDefs.push_back(CrtShadersCrtRoyaleSrcCrtRoyaleScanlinesVerticalInterlacingShaderDef()
.Param("filter_linear", "true")
.Param("scale_type_x", "source")
.Param("scale_type_y", "source")
.Param("scale_x", "1.0")
.Param("scale_y", "3.0")
.Param("srgb_framebuffer", "true"));
         	ShaderDefs.push_back(CrtShadersCrtMaximusRoyaleSrcResizeShaderDef()
.Param("alias", "VERTICAL_SCANLINES")
.Param("filter_linear", "true")
.Param("scale_type_x", "source")
.Param("scale_type_y", "viewport")
.Param("scale_x", "1.0")
.Param("scale_y", "1.95")
.Param("srgb_framebuffer", "true"));
         	ShaderDefs.push_back(CrtShadersCrtRoyaleSrcCrtRoyaleBloomApproxShaderDef()
.Param("filter_linear", "true")
.Param("scale_type_x", "absolute")
.Param("scale_type_y", "absolute")
.Param("scale_x", "320")
.Param("scale_y", "240")
.Param("srgb_framebuffer", "true"));
         	ShaderDefs.push_back(CrtShadersCrtMaximusRoyaleSrcResizeShaderDef()
.Param("alias", "BLOOM_APPROX")
.Param("filter_linear", "true")
.Param("scale", "1.0")
.Param("scale_type", "source")
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
         	ShaderDefs.push_back(CrtShadersCrtRoyaleSrcCrtRoyaleMaskResizeVerticalShaderDef()
.Param("filter_linear", "true")
.Param("scale_type_x", "absolute")
.Param("scale_type_y", "viewport")
.Param("scale_x", "128")
.Param("scale_y", "0.975")
.Param("srgb_framebuffer", "false"));
         	ShaderDefs.push_back(CrtShadersCrtRoyaleSrcCrtRoyaleMaskResizeHorizontalShaderDef()
.Param("alias", "MASK_RESIZE")
.Param("filter_linear", "false")
.Param("scale_type_x", "viewport")
.Param("scale_type_y", "source")
.Param("scale_x", "0.73125")
.Param("scale_y", "1.0")
.Param("srgb_framebuffer", "true"));
         	ShaderDefs.push_back(CrtShadersCrtRoyaleSrcCrtRoyaleScanlinesHorizontalApplyMaskShaderDef()
.Param("alias", "MASKED_SCANLINES")
.Param("filter_linear", "true")
.Param("scale", "1.0")
.Param("scale_type", "source")
.Param("srgb_framebuffer", "true"));
         	ShaderDefs.push_back(CrtShadersCrtRoyaleSrcCrtRoyaleBrightpassShaderDef()
.Param("alias", "BRIGHTPASS")
.Param("filter_linear", "true")
.Param("scale", "1.0")
.Param("scale_type", "source")
.Param("srgb_framebuffer", "true"));
         	ShaderDefs.push_back(CrtShadersCrtRoyaleSrcCrtRoyaleBloomVerticalShaderDef()
.Param("filter_linear", "true")
.Param("scale", "1.0")
.Param("scale_type", "source")
.Param("srgb_framebuffer", "true"));
         	ShaderDefs.push_back(CrtShadersCrtRoyaleSrcCrtRoyaleBloomHorizontalReconstituteShaderDef()
.Param("filter_linear", "true")
.Param("scale", "1.0")
.Param("scale_type", "source")
.Param("srgb_framebuffer", "true"));
         	ShaderDefs.push_back(CrtShadersCrtRoyaleSrcCrtRoyaleGeometryAaLastPassShaderDef()
.Param("alias", "LAST_PASS")
.Param("filter_linear", "true")
.Param("mipmap_input", "true")
.Param("scale", "1.0")
.Param("scale_type", "source")
.Param("srgb_framebuffer", "true"));
         	ShaderDefs.push_back(CrtShadersCrtMaximusRoyaleSrcCrt_reflectShaderDef()
.Param("alias", "BORDER_REFLECTION")
.Param("filter_linear", "true")
.Param("scale_type_x", "absolute")
.Param("scale_type_y", "absolute")
.Param("scale_x", "160")
.Param("scale_y", "120")
.Param("srgb_framebuffer", "false"));
         	ShaderDefs.push_back(CrtShadersCrtMaximusRoyaleSrcBGcolor_pass1ShaderDef()
.Param("alias", "BGPASS1")
.Param("filter_linear", "false")
.Param("scale_type_x", "absolute")
.Param("scale_type_y", "absolute")
.Param("scale_x", "1.0")
.Param("scale_y", "100.0")
.Param("srgb_framebuffer", "false"));
         	ShaderDefs.push_back(CrtShadersCrtMaximusRoyaleSrcBGcolor_pass2ShaderDef()
.Param("alias", "BGPASS2")
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "absolute")
.Param("srgb_framebuffer", "false"));
         	ShaderDefs.push_back(CrtShadersCrtMaximusRoyaleSrcImgborderShaderDef()
.Param("filter_linear", "true")
.Param("scale", "1.0")
.Param("scale_type", "viewport"));
         	ShaderDefs.push_back(CrtShadersCrtMaximusRoyaleSrcTv_reflectionShaderDef()
.Param("filter_linear", "true")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(StockStockShaderDef()
.Param("scale_type", "viewport"));
            TextureDefs.push_back(CrtShadersCrtMaximusRoyaleTilePhosphorTexturesTilePhosphor128pxTextureDef()
.Param("linear", "true")
.Param("mipmap", "false")
.Param("name", "mask_grille_texture_small")
.Param("wrap_mode", "repeat"));
            TextureDefs.push_back(CrtShadersCrtMaximusRoyaleTilePhosphorTexturesTilePhosphor512pxTextureDef()
.Param("linear", "true")
.Param("mipmap", "true")
.Param("name", "mask_grille_texture_large")
.Param("wrap_mode", "repeat"));
            TextureDefs.push_back(CrtShadersCrtMaximusRoyaleTilePhosphorTexturesTilePhosphorMatrix128pxTextureDef()
.Param("linear", "true")
.Param("mipmap", "false")
.Param("name", "mask_slot_texture_small")
.Param("wrap_mode", "repeat"));
            TextureDefs.push_back(CrtShadersCrtMaximusRoyaleTilePhosphorTexturesTilePhosphorMatrix512pxTextureDef()
.Param("linear", "true")
.Param("mipmap", "true")
.Param("name", "mask_slot_texture_large")
.Param("wrap_mode", "repeat"));
            TextureDefs.push_back(CrtShadersCrtMaximusRoyaleTilePhosphorTexturesTilePhosphorDots128pxTextureDef()
.Param("linear", "true")
.Param("mipmap", "false")
.Param("name", "mask_shadow_texture_small")
.Param("wrap_mode", "repeat"));
            TextureDefs.push_back(CrtShadersCrtMaximusRoyaleTilePhosphorTexturesTilePhosphorDots512pxTextureDef()
.Param("linear", "true")
.Param("mipmap", "true")
.Param("name", "mask_shadow_texture_large")
.Param("wrap_mode", "repeat"));
            TextureDefs.push_back(CrtShadersCrtMaximusRoyaleFrameTextures16_9TV_frame_1TextureDef()
.Param("name", "TVBORDER1"));
            TextureDefs.push_back(CrtShadersCrtMaximusRoyaleFrameTextures16_9TV_frame_2TextureDef()
.Param("name", "TVBORDER2"));
            TextureDefs.push_back(CrtShadersCrtMaximusRoyaleFrameTextures16_9TV_frame_3TextureDef()
.Param("name", "TVBORDER3"));
            TextureDefs.push_back(CrtShadersCrtMaximusRoyaleFrameTextures16_9TV_frame_2_bgTextureDef()
.Param("name", "TVBORDER2BG"));
            TextureDefs.push_back(CrtShadersCrtMaximusRoyaleFrameTextures16_9TV_frame_3_bgTextureDef()
.Param("name", "TVBORDER3BG"));
            TextureDefs.push_back(CrtShadersCrtMaximusRoyaleFrameTextures16_9TV_frame_2_ledTextureDef()
.Param("name", "TVBORDER2LED"));
            TextureDefs.push_back(CrtShadersCrtMaximusRoyaleFrameTextures16_9TV_frame_3_ledTextureDef()
.Param("name", "TVBORDER3LED"));
            TextureDefs.push_back(CrtShadersCrtMaximusRoyaleFrameTextures16_9TV_frame_1_lights_alphaTextureDef()
.Param("name", "TVLIGHTS1"));
            TextureDefs.push_back(CrtShadersCrtMaximusRoyaleFrameTextures16_9TV_frame_3_lights_alphaTextureDef()
.Param("name", "TVLIGHTS3"));
            TextureDefs.push_back(CrtShadersCrtMaximusRoyaleFrameTextures16_9MONITOR_frameTextureDef()
.Param("name", "MONITORBORDER"));
            TextureDefs.push_back(CrtShadersCrtMaximusRoyaleFrameTextures16_9MONITOR_frame_lights_alphaTextureDef()
.Param("name", "MONITORLIGHTS"));
            TextureDefs.push_back(CrtShadersCrtMaximusRoyaleFrameTextures16_9Frame_1_shapeTextureDef()
.Param("name", "SHAPE1"));
            TextureDefs.push_back(CrtShadersCrtMaximusRoyaleFrameTextures16_9Frame_2_shapeTextureDef()
.Param("name", "SHAPE2"));
	}
};
}
