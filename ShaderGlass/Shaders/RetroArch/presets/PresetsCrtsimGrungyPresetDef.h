/*
ShaderGlass preset presets / crtsim-grungy imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/presets/crtsim-grungy.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class PresetsCrtsimGrungyPresetDef : public PresetDef
{
public:
	PresetsCrtsimGrungyPresetDef() : PresetDef{}
	{
		Name = "crtsim-grungy";
		Category = "presets";
	}

	virtual void Build() {
         	ShaderDefs.push_back(CrtShadersCrtsimCompositeShaderDef()
.Param("filter_linear", "false")
.Param("float_framebuffer", "false")
.Param("mipmap_input", "false")
.Param("srgb_framebuffer", "false")
.Param("wrap_mode", "clamp_to_border"));
         	ShaderDefs.push_back(MiscShadersImageAdjustmentShaderDef()
.Param("alias", "CRTPASS")
.Param("float_framebuffer", "false")
.Param("mipmap_input", "false")
.Param("srgb_framebuffer", "false")
.Param("wrap_mode", "clamp_to_border"));
         	ShaderDefs.push_back(CrtShadersCrtsimPostDownsampleShaderDef()
.Param("filter_linear", "true")
.Param("float_framebuffer", "false")
.Param("mipmap_input", "false")
.Param("srgb_framebuffer", "false")
.Param("wrap_mode", "clamp_to_border"));
         	ShaderDefs.push_back(CrtShadersCrtsimPostUpsampleShaderDef()
.Param("filter_linear", "true")
.Param("float_framebuffer", "false")
.Param("mipmap_input", "false")
.Param("srgb_framebuffer", "false")
.Param("wrap_mode", "clamp_to_border"));
         	ShaderDefs.push_back(CrtShadersCrtsimPresentShaderDef()
.Param("filter_linear", "true")
.Param("float_framebuffer", "false")
.Param("mipmap_input", "false")
.Param("srgb_framebuffer", "false")
.Param("wrap_mode", "clamp_to_border"));
            TextureDefs.push_back(CrtShadersCrtsimArtifactsTextureDef()
.Param("linear", "true")
.Param("mipmap", "false")
.Param("name", "NTSCArtifactSampler")
.Param("wrap_mode", "clamp_to_border"));
            TextureDefs.push_back(CrtShadersCrtsimMaskTextureDef()
.Param("linear", "true")
.Param("mipmap", "false")
.Param("name", "shadowMaskSampler")
.Param("wrap_mode", "clamp_to_border"));
            OverrideParam("BloomPower", (float)1.000000);
            OverrideParam("BloomScalar", (float)0.100000);
            OverrideParam("CRTMask_Scale", (float)1.000000);
            OverrideParam("GRAIN_STR", (float)3.000000);
            OverrideParam("NTSCArtifactScale", (float)200.000000);
            OverrideParam("NTSCLerp", (float)1.000000);
            OverrideParam("Tuning_Artifacts", (float)0.700000);
            OverrideParam("Tuning_Barrel", (float)0.250000);
            OverrideParam("Tuning_Bleed", (float)1.000000);
            OverrideParam("Tuning_Mask_Brightness", (float)0.600000);
            OverrideParam("Tuning_Mask_Opacity", (float)0.500000);
            OverrideParam("Tuning_Overscan", (float)0.950000);
            OverrideParam("Tuning_Persistence_B", (float)0.060000);
            OverrideParam("Tuning_Persistence_G", (float)0.060000);
            OverrideParam("Tuning_Persistence_R", (float)0.075000);
            OverrideParam("Tuning_Satur", (float)0.900000);
            OverrideParam("Tuning_Sharp", (float)0.200000);
            OverrideParam("animate_artifacts", (float)1.000000);
            OverrideParam("bloom_scale_down", (float)0.015000);
            OverrideParam("bloom_scale_up", (float)0.020000);
            OverrideParam("mask_toggle", (float)1.000000);
	}
};
}
