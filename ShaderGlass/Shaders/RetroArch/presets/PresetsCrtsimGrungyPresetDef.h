/*
ShaderGlass preset presets / crtsim-grungy imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/presets/crtsim-grungy.slangp
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
.Param("srgb_framebuffer", "false"));
         	ShaderDefs.push_back(MiscImageAdjustmentShaderDef()
.Param("alias", "CRTPASS")
.Param("float_framebuffer", "false")
.Param("mipmap_input", "false")
.Param("srgb_framebuffer", "false"));
         	ShaderDefs.push_back(CrtShadersCrtsimPostDownsampleShaderDef()
.Param("filter_linear", "true")
.Param("float_framebuffer", "false")
.Param("mipmap_input", "false")
.Param("srgb_framebuffer", "false"));
         	ShaderDefs.push_back(CrtShadersCrtsimPostUpsampleShaderDef()
.Param("filter_linear", "true")
.Param("float_framebuffer", "false")
.Param("mipmap_input", "false")
.Param("srgb_framebuffer", "false"));
         	ShaderDefs.push_back(CrtShadersCrtsimPresentShaderDef()
.Param("filter_linear", "true")
.Param("float_framebuffer", "false")
.Param("mipmap_input", "false")
.Param("srgb_framebuffer", "false"));
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
	}
};
}
