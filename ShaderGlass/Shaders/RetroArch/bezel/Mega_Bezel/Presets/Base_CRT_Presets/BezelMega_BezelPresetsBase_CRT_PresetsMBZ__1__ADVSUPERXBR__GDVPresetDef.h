/*
ShaderGlass preset bezel/Mega_Bezel_Base / MBZ__1__ADV-SUPER-XBR__GDV imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/bezel/Mega_Bezel/Presets/Base_CRT_Presets/MBZ__1__ADV-SUPER-XBR__GDV.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class BezelMega_BezelPresetsBase_CRT_PresetsMBZ__1__ADVSUPERXBR__GDVPresetDef : public PresetDef
{
public:
	BezelMega_BezelPresetsBase_CRT_PresetsMBZ__1__ADVSUPERXBR__GDVPresetDef() : PresetDef{}
	{
		Name = "MBZ__1__ADV-SUPER-XBR__GDV";
		Category = "bezel/Mega_Bezel_Base";
	}

	virtual void Build() {
         	ShaderDefs.push_back(BezelMega_BezelShadersGuestExtrasHsmDrezNoneShaderDef()
.Param("alias", "DerezedPass")
.Param("filter_linear", "false")
.Param("scale_type", "source")
.Param("scale_x", "1")
.Param("scale_y", "1")
.Param("srgb_framebuffer", "true"));
         	ShaderDefs.push_back(BezelMega_BezelShadersBaseCacheInfoAllParamsShaderDef()
.Param("alias", "InfoCachePass")
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(BezelMega_BezelShadersBaseTextAdvShaderDef()
.Param("alias", "TextPass")
.Param("filter_linear", "false")
.Param("float_framebuffer", "true")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(BezelMega_BezelShadersGuestExtrasHsmFetchDrezOutputShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "source")
.Param("scale_x", "1")
.Param("scale_y", "1")
.Param("srgb_framebuffer", "true"));
         	ShaderDefs.push_back(BezelMega_BezelShadersDeditherDeditherGammaPrep1BeforeShaderDef()
.Param("alias", "LinearGamma"));
         	ShaderDefs.push_back(BezelMega_BezelShadersHyllianSgenptMixSgenptMixPass1ShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(BezelMega_BezelShadersHyllianSgenptMixSgenptMixPass2ShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(BezelMega_BezelShadersHyllianSgenptMixSgenptMixPass3ShaderDef()
.Param("alias", "CB_Output")
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(BezelMega_BezelShadersHyllianSgenptMixSgenptMixPass4ShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(BezelMega_BezelShadersHyllianSgenptMixSgenptMixPass5ShaderDef()
.Param("filter_linear", "false"));
         	ShaderDefs.push_back(BezelMega_BezelShadersDeditherDeditherGammaPrep2AfterShaderDef());
         	ShaderDefs.push_back(BezelMega_BezelShadersPs1ditherHsmPS1UnditherBoxBlurShaderDef());
         	ShaderDefs.push_back(BezelMega_BezelShadersFxaaFxaaShaderDef()
.Param("float_framebuffer", "true"));
         	ShaderDefs.push_back(BezelMega_BezelShadersGuestExtrasHsmGSharp_resamplerShaderDef());
         	ShaderDefs.push_back(BezelMega_BezelShadersGuestExtrasHsmSharpsmootherShaderDef()
.Param("alias", "DeditherPass"));
         	ShaderDefs.push_back(BezelMega_BezelShadersBaseStockShaderDef()
.Param("alias", "XbrSource"));
         	ShaderDefs.push_back(BezelMega_BezelShadersHyllianCrtSuperXbrSuperXbrPass0ShaderDef()
.Param("filter_linear", "false")
.Param("float_framebuffer", "false")
.Param("mipmap_input", "false")
.Param("srgb_framebuffer", "true")
.Param("wrap_mode", "clamp_to_border"));
         	ShaderDefs.push_back(BezelMega_BezelShadersHyllianCrtSuperXbrSuperXbrPass1ShaderDef()
.Param("filter_linear", "false")
.Param("float_framebuffer", "false")
.Param("mipmap_input", "false")
.Param("scale_type_x", "source")
.Param("scale_type_y", "source")
.Param("scale_x", "2.000000")
.Param("scale_y", "2.000000")
.Param("srgb_framebuffer", "true")
.Param("wrap_mode", "clamp_to_border"));
         	ShaderDefs.push_back(BezelMega_BezelShadersHyllianCrtSuperXbrSuperXbrPass2ShaderDef()
.Param("filter_linear", "false")
.Param("float_framebuffer", "false")
.Param("mipmap_input", "false")
.Param("scale_type_x", "source")
.Param("scale_type_y", "source")
.Param("scale_x", "1.000000")
.Param("scale_y", "1.000000")
.Param("srgb_framebuffer", "true")
.Param("wrap_mode", "clamp_to_border"));
         	ShaderDefs.push_back(BezelMega_BezelShadersHyllianCrtSuperXbrCustomBicubicXShaderDef()
.Param("filter_linear", "false")
.Param("float_framebuffer", "false")
.Param("mipmap_input", "false")
.Param("scale_type_x", "source")
.Param("scale_type_y", "source")
.Param("scale_x", "1.000000")
.Param("scale_y", "1.000000")
.Param("srgb_framebuffer", "true")
.Param("wrap_mode", "clamp_to_border"));
         	ShaderDefs.push_back(BezelMega_BezelShadersHyllianCrtSuperXbrCustomBicubicYShaderDef()
.Param("filter_linear", "false")
.Param("scale_type_x", "source")
.Param("scale_type_y", "source")
.Param("scale_x", "1")
.Param("scale_y", "0.5")
.Param("wrap_mode", "clamp_to_edge"));
         	ShaderDefs.push_back(BezelMega_BezelShadersBaseIntroShaderDef()
.Param("alias", "IntroPass")
.Param("filter_linear", "false")
.Param("float_framebuffer", "true")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(BezelMega_BezelShadersGtuHsmGtuPass1ShaderDef()
.Param("float_framebuffer", "true")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(BezelMega_BezelShadersGtuHsmGtuPass2ShaderDef()
.Param("filter_linear", "false")
.Param("float_framebuffer", "true")
.Param("scale_type_x", "source")
.Param("scale_type_y", "source")
.Param("scale_x", "1.0")
.Param("scale_y", "1.0"));
         	ShaderDefs.push_back(BezelMega_BezelShadersBaseStockShaderDef()
.Param("alias", "PreCRTPass"));
         	ShaderDefs.push_back(BezelMega_BezelShadersGuestHsmAfterglow0ShaderDef()
.Param("alias", "AfterglowPass")
.Param("filter_linear", "true")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(BezelMega_BezelShadersGuestHsmPreShadersAfterglowShaderDef()
.Param("filter_linear", "true")
.Param("mipmap_input", "true")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(BezelMega_BezelShadersDogwayHsmGradeShaderDef()
.Param("alias", "ColorCorrectPass")
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(BezelMega_BezelShadersGuestHsmCustomFastSharpenShaderDef()
.Param("filter_linear", "true")
.Param("scale_type", "source")
.Param("scale_x", "1.0")
.Param("scale_y", "1.0"));
         	ShaderDefs.push_back(BezelMega_BezelShadersBaseStockShaderDef()
.Param("alias", "PrePass")
.Param("filter_linear", "true")
.Param("mipmap_input", "true")
.Param("scale_type", "source")
.Param("scale_x", "1.0")
.Param("scale_y", "1.0"));
         	ShaderDefs.push_back(BezelMega_BezelShadersGuestHsmAvgLumShaderDef()
.Param("alias", "AvgLumPass")
.Param("filter_linear", "true")
.Param("mipmap_input", "true")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(BezelMega_BezelShadersGuestHsmInterlaceAndLinearizeShaderDef()
.Param("alias", "LinearizePass")
.Param("filter_linear", "true")
.Param("float_framebuffer", "true")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(BezelMega_BezelShadersGuestHsmGaussian_horizontalShaderDef()
.Param("filter_linear", "true")
.Param("float_framebuffer", "true")
.Param("scale_type_x", "absolute")
.Param("scale_type_y", "source")
.Param("scale_x", "800.0")
.Param("scale_y", "1.0"));
         	ShaderDefs.push_back(BezelMega_BezelShadersGuestHsmGaussian_verticalShaderDef()
.Param("alias", "GlowPass")
.Param("filter_linear", "true")
.Param("float_framebuffer", "true")
.Param("scale_type_x", "absolute")
.Param("scale_type_y", "absolute")
.Param("scale_x", "800.0")
.Param("scale_y", "600.0"));
         	ShaderDefs.push_back(BezelMega_BezelShadersGuestHsmBloom_horizontalShaderDef()
.Param("filter_linear", "true")
.Param("float_framebuffer", "true")
.Param("scale_type_x", "absolute")
.Param("scale_type_y", "absolute")
.Param("scale_x", "800.0")
.Param("scale_y", "600.0"));
         	ShaderDefs.push_back(BezelMega_BezelShadersGuestHsmBloom_verticalShaderDef()
.Param("alias", "BloomPass")
.Param("filter_linear", "true")
.Param("float_framebuffer", "true")
.Param("scale_type_x", "source")
.Param("scale_type_y", "source")
.Param("scale_x", "1.0")
.Param("scale_y", "1.0"));
         	ShaderDefs.push_back(BezelMega_BezelShadersGuestHsmCrtGuestAdvancedShaderDef()
.Param("filter_linear", "true")
.Param("float_framebuffer", "true")
.Param("scale_type", "viewport")
.Param("scale_x", "1.0")
.Param("scale_y", "1.0"));
         	ShaderDefs.push_back(BezelMega_BezelShadersGuestHsmDeconvergenceShaderDef()
.Param("alias", "CRTPass")
.Param("filter_linear", "true")
.Param("float_framebuffer", "true")
.Param("scale_type", "viewport")
.Param("scale_x", "1.0")
.Param("scale_y", "1.0"));
         	ShaderDefs.push_back(BezelMega_BezelShadersBasePostCrtPrepImageLayersShaderDef()
.Param("alias", "PostCRTPass")
.Param("float_framebuffer", "true")
.Param("mipmap_input", "true")
.Param("scale_type", "viewport"));
         	ShaderDefs.push_back(BezelMega_BezelShadersBaseBezelImagesUnderCrtShaderDef()
.Param("alias", "BR_LayersUnderCRTPass")
.Param("filter_linear", "true")
.Param("float_framebuffer", "true")
.Param("scale_type", "viewport"));
         	ShaderDefs.push_back(BezelMega_BezelShadersBaseBezelImagesOverCrtShaderDef()
.Param("alias", "BR_LayersOverCRTPass")
.Param("filter_linear", "true")
.Param("float_framebuffer", "true")
.Param("scale_type", "viewport"));
         	ShaderDefs.push_back(BezelMega_BezelShadersBaseLinearizeCrtShaderDef()
.Param("alias", "BR_MirrorLowResPass")
.Param("filter_linear", "true")
.Param("float_framebuffer", "true")
.Param("mipmap_input", "true")
.Param("scale_type", "absolute")
.Param("scale_x", "800")
.Param("scale_y", "600"));
         	ShaderDefs.push_back(BezelMega_BezelShadersBaseBlurOutsideScreenHorizShaderDef()
.Param("filter_linear", "true")
.Param("float_framebuffer", "true")
.Param("mipmap_input", "true"));
         	ShaderDefs.push_back(BezelMega_BezelShadersBaseBlurOutsideScreenVertShaderDef()
.Param("alias", "BR_MirrorBlurredPass")
.Param("filter_linear", "true")
.Param("float_framebuffer", "true"));
         	ShaderDefs.push_back(BlursShadersRoyaleBlur9x9ShaderDef()
.Param("alias", "BR_MirrorReflectionDiffusedPass")
.Param("filter_linear", "true")
.Param("float_framebuffer", "true")
.Param("mipmap_input", "true")
.Param("scale_type", "absolute")
.Param("scale_x", "128")
.Param("scale_y", "128"));
         	ShaderDefs.push_back(BlursShadersRoyaleBlur9x9ShaderDef()
.Param("alias", "BR_MirrorFullscreenGlowPass")
.Param("filter_linear", "true")
.Param("float_framebuffer", "true")
.Param("mipmap_input", "true")
.Param("scale_type", "absolute")
.Param("scale_x", "12")
.Param("scale_y", "12"));
         	ShaderDefs.push_back(BezelMega_BezelShadersBaseReflectionShaderDef()
.Param("alias", "ReflectionPass")
.Param("scale_type", "viewport"));
         	ShaderDefs.push_back(BezelMega_BezelShadersBaseCombinePassesShaderDef()
.Param("alias", "CombinePass"));
            TextureDefs.push_back(BezelMega_BezelShadersGuestLutTrinitronLutTextureDef()
.Param("linear", "true")
.Param("name", "SamplerLUT1"));
            TextureDefs.push_back(BezelMega_BezelShadersGuestLutInvTrinitronLutTextureDef()
.Param("linear", "true")
.Param("name", "SamplerLUT2"));
            TextureDefs.push_back(BezelMega_BezelShadersGuestLutNecLutTextureDef()
.Param("linear", "true")
.Param("name", "SamplerLUT3"));
            TextureDefs.push_back(BezelMega_BezelShadersGuestLutNtscLutTextureDef()
.Param("linear", "true")
.Param("name", "SamplerLUT4"));
            TextureDefs.push_back(BezelMega_BezelShadersTexturesIntroImage_MegaBezelLogoTextureDef()
.Param("linear", "true")
.Param("mipmap", "1")
.Param("name", "IntroImage"));
            TextureDefs.push_back(BezelMega_BezelShadersTexturesPlaceholder_Transparent_16x16TextureDef()
.Param("linear", "false")
.Param("name", "ScreenPlacementImage"));
            TextureDefs.push_back(BezelMega_BezelShadersTexturesTube_Diffuse_2390x1792TextureDef()
.Param("linear", "true")
.Param("mipmap", "1")
.Param("name", "TubeDiffuseImage"));
            TextureDefs.push_back(BezelMega_BezelShadersTexturesColored_Gel_RainbowTextureDef()
.Param("linear", "true")
.Param("mipmap", "1")
.Param("name", "TubeColoredGelImage"));
            TextureDefs.push_back(BezelMega_BezelShadersTexturesTube_Shadow_1600x1200TextureDef()
.Param("linear", "true")
.Param("mipmap", "1")
.Param("name", "TubeShadowImage"));
            TextureDefs.push_back(BezelMega_BezelShadersTexturesTubeGlassOverlayImageCropped_1440x1080TextureDef()
.Param("linear", "true")
.Param("mipmap", "1")
.Param("name", "TubeStaticReflectionImage"));
            TextureDefs.push_back(BezelMega_BezelShadersTexturesBackgroundImage_Carbon_3840x2160TextureDef()
.Param("linear", "true")
.Param("mipmap", "1")
.Param("name", "BackgroundImage"));
            TextureDefs.push_back(BezelMega_BezelShadersTexturesPlaceholder_Transparent_16x16TextureDef()
.Param("linear", "true")
.Param("mipmap", "1")
.Param("name", "BackgroundVertImage"));
            TextureDefs.push_back(BezelMega_BezelShadersTexturesPlaceholder_White_16x16TextureDef()
.Param("linear", "true")
.Param("mipmap", "1")
.Param("name", "ReflectionMaskImage"));
            TextureDefs.push_back(BezelMega_BezelShadersTexturesFrameTexture_2800x2120TextureDef()
.Param("linear", "true")
.Param("mipmap", "1")
.Param("name", "FrameTextureImage"));
            TextureDefs.push_back(BezelMega_BezelShadersTexturesPlaceholder_Transparent_16x16TextureDef()
.Param("linear", "true")
.Param("mipmap", "1")
.Param("name", "CabinetGlassImage"));
            TextureDefs.push_back(BezelMega_BezelShadersTexturesPlaceholder_Transparent_16x16TextureDef()
.Param("linear", "true")
.Param("mipmap", "1")
.Param("name", "DeviceImage"));
            TextureDefs.push_back(BezelMega_BezelShadersTexturesPlaceholder_Transparent_16x16TextureDef()
.Param("linear", "true")
.Param("mipmap", "1")
.Param("name", "DeviceVertImage"));
            TextureDefs.push_back(BezelMega_BezelShadersTexturesPlaceholder_Transparent_16x16TextureDef()
.Param("linear", "true")
.Param("mipmap", "1")
.Param("name", "DeviceLEDImage"));
            TextureDefs.push_back(BezelMega_BezelShadersTexturesPlaceholder_Transparent_16x16TextureDef()
.Param("linear", "true")
.Param("mipmap", "1")
.Param("name", "DecalImage"));
            TextureDefs.push_back(BezelMega_BezelShadersTexturesNightLightingClose_1920x1080TextureDef()
.Param("linear", "true")
.Param("mipmap", "1")
.Param("name", "NightLightingImage"));
            TextureDefs.push_back(BezelMega_BezelShadersTexturesNightLightingFar_1920x1080TextureDef()
.Param("linear", "true")
.Param("mipmap", "1")
.Param("name", "NightLighting2Image"));
            TextureDefs.push_back(BezelMega_BezelShadersTexturesPlaceholder_Transparent_16x16TextureDef()
.Param("linear", "true")
.Param("mipmap", "1")
.Param("name", "LEDImage"));
            TextureDefs.push_back(BezelMega_BezelShadersTexturesPlaceholder_Transparent_16x16TextureDef()
.Param("linear", "true")
.Param("mipmap", "1")
.Param("name", "TopLayerImage"));
            OverrideParam("HSM_CORE_RES_SAMPLING_MULT_OPPOSITE_DIR", (float)100.000000);
            OverrideParam("HSM_CORE_RES_SAMPLING_MULT_SCANLINE_DIR", (float)600.000000);
            OverrideParam("HSM_DOWNSAMPLE_BLUR_OPPOSITE_DIR", (float)0.000000);
            OverrideParam("HSM_DOWNSAMPLE_BLUR_SCANLINE_DIR", (float)0.000000);
            OverrideParam("SGPT_BLEND_OPTION", (float)1.000000);
            OverrideParam("SHARPEN", (float)1.000000);
            OverrideParam("SHARPSMOOTHER_ON", (float)1.000000);
            OverrideParam("SUPERXBR_ON", (float)1.000000);
            OverrideParam("lumad", (float)0.500000);
            OverrideParam("masksize", (float)0.000000);
            OverrideParam("max_w", (float)0.050000);
            OverrideParam("min_w", (float)0.000000);
            OverrideParam("mtric", (float)0.300000);
            OverrideParam("smoot", (float)0.400000);
	}
};
}
