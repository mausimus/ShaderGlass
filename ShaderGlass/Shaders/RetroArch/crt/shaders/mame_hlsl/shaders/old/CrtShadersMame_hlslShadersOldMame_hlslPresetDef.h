/*
ShaderGlass preset crt-shaders-mame_hlsl-shaders-old / mame_hlsl imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/crt/shaders/mame_hlsl/shaders/old/mame_hlsl.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class CrtShadersMame_hlslShadersOldMame_hlslPresetDef : public PresetDef
{
public:
	CrtShadersMame_hlslShadersOldMame_hlslPresetDef() : PresetDef{}
	{
		Name = "mame_hlsl";
		Category = "crt-shaders-mame_hlsl-shaders-old";
	}

	virtual void Build() {
         	ShaderDefs.push_back(CrtShadersMame_hlslShadersOldShadersMame_hlslShadersMame_ntscShaderDef()
.Param("alias", "NTSCPass")
.Param("filter_linear", "true")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(CrtShadersMame_hlslShadersOldShadersMame_hlslShadersMame_colorShaderDef()
.Param("alias", "ColorPass")
.Param("filter_linear", "true")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(PixelArtScalingShadersSharpBilinearShaderDef()
.Param("alias", "PrescalePass")
.Param("filter_linear", "true")
.Param("scale", "4.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(CrtShadersMame_hlslShadersOldShadersMame_hlslShadersMame_deconvergeShaderDef()
.Param("alias", "DeconvergePass")
.Param("filter_linear", "true")
.Param("scale", "0.5")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(CrtShadersMame_hlslShadersOldShadersMame_hlslShadersMame_scanlineShaderDef()
.Param("alias", "ScanlinePass")
.Param("filter_linear", "true")
.Param("scale", "2.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(CrtShadersMame_hlslShadersOldShadersMame_hlslShadersMame_focusShaderDef()
.Param("alias", "FocusPass")
.Param("filter_linear", "true"));
         	ShaderDefs.push_back(CrtShadersMame_hlslShadersOldShadersMame_hlslShadersMame_phosphorShaderDef()
.Param("alias", "PhosphorPass")
.Param("filter_linear", "true"));
         	ShaderDefs.push_back(CrtShadersMame_hlslShadersOldShadersMame_hlslShadersMame_postShaderDef()
.Param("alias", "PostPass")
.Param("filter_linear", "true")
.Param("scale_type", "viewport"));
         	ShaderDefs.push_back(CrtShadersMame_hlslShadersOldShadersMame_hlslShadersMame_chromaShaderDef()
.Param("alias", "ChromaPass")
.Param("filter_linear", "true")
.Param("scale_type", "viewport"));
         	ShaderDefs.push_back(CrtShadersMame_hlslShadersOldShadersMame_hlslShadersMame_downsampleShaderDef()
.Param("alias", "DownsamplePass")
.Param("filter_linear", "true"));
         	ShaderDefs.push_back(CrtShadersMame_hlslShadersOldShadersMame_hlslShadersMame_bloomShaderDef()
.Param("alias", "BloomPass")
.Param("filter_linear", "true")
.Param("mipmap_input", "true")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(CrtShadersMame_hlslShadersOldShadersMame_hlslShadersMame_distortionShaderDef()
.Param("alias", "DistortionPass")
.Param("filter_linear", "true"));
            TextureDefs.push_back(CrtShadersMame_hlslResourcesApertureGrilleTextureDef()
.Param("name", "MaskTexture"));
	}
};
}
