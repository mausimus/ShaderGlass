/*
ShaderGlass preset presets/crt-plus-signal / ntsclut-phosphorlut imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/presets/crt-plus-signal/ntsclut-phosphorlut.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class PresetsCrtPlusSignalNtsclutPhosphorlutPresetDef : public PresetDef
{
public:
	PresetsCrtPlusSignalNtsclutPhosphorlutPresetDef() : PresetDef{}
	{
		Name = "ntsclut-phosphorlut";
		Category = "presets/crt-plus-signal";
	}

	virtual void Build() {
         	ShaderDefs.push_back(CrtShadersCrtsimCompositeShaderDef()
.Param("filter_linear", "false"));
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
.Param("filter_linear", "true"));
            TextureDefs.push_back(CrtShadersCrtsimArtifactsTextureDef()
.Param("linear", "true")
.Param("name", "NTSCArtifactSampler"));
            TextureDefs.push_back(CrtShadersPhosphorlutLutsShadowmaskTextureDef()
.Param("name", "shadow"));
            TextureDefs.push_back(CrtShadersPhosphorlutLutsApertureGrilleTextureDef()
.Param("name", "aperture"));
            TextureDefs.push_back(CrtShadersPhosphorlutLutsSlotmaskTextureDef()
.Param("name", "slot"));
	}
};
}
