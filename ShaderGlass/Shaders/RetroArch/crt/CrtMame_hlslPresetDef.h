/*
ShaderGlass preset crt / mame_hlsl imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/6f921ee4815a7894a33855974285b04545a4fa42/crt/mame_hlsl.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class CrtMame_hlslPresetDef : public PresetDef
{
public:
	CrtMame_hlslPresetDef() : PresetDef{}
	{
		Name = "mame_hlsl";
		Category = "crt";
	}

	virtual void Build() {
         	ShaderDefs.push_back(CrtShadersMame_hlslShadersMame_ntsc_encodeShaderDef()
.Param("filter_linear", "true")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(CrtShadersMame_hlslShadersMame_ntsc_decodeShaderDef()
.Param("alias", "NTSCPass")
.Param("filter_linear", "true")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(CrtShadersMame_hlslShadersMame_colorShaderDef()
.Param("alias", "ColorPass")
.Param("filter_linear", "true")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(CrtShadersMame_hlslShadersMame_prescaleShaderDef()
.Param("alias", "PrescalePass")
.Param("filter_linear", "true")
.Param("scale", "1.0")
.Param("scale_type", "viewport"));
         	ShaderDefs.push_back(CrtShadersMame_hlslShadersMame_deconvergeShaderDef()
.Param("alias", "DeconvergePass")
.Param("filter_linear", "true")
.Param("scale", "1.0")
.Param("scale_type", "viewport"));
         	ShaderDefs.push_back(CrtShadersMame_hlslShadersMame_scanlineShaderDef()
.Param("alias", "ScanlinePass")
.Param("filter_linear", "true")
.Param("scale", "1.0")
.Param("scale_type", "viewport"));
         	ShaderDefs.push_back(CrtShadersMame_hlslShadersMame_defocusShaderDef()
.Param("alias", "FocusPass")
.Param("filter_linear", "true")
.Param("scale", "1.0")
.Param("scale_type", "viewport"));
         	ShaderDefs.push_back(CrtShadersMame_hlslShadersMame_postShaderDef()
.Param("alias", "PostPass")
.Param("filter_linear", "true")
.Param("scale", "1.0")
.Param("scale_type", "viewport"));
         	ShaderDefs.push_back(CrtShadersMame_hlslShadersMame_phosphorShaderDef()
.Param("alias", "PhosphorPass")
.Param("filter_linear", "true")
.Param("scale", "1.0")
.Param("scale_type", "viewport"));
         	ShaderDefs.push_back(CrtShadersMame_hlslShadersMame_chromaShaderDef()
.Param("alias", "ChromaPass")
.Param("filter_linear", "true")
.Param("scale", "1.0")
.Param("scale_type", "viewport"));
         	ShaderDefs.push_back(CrtShadersMame_hlslShadersMame_distortionShaderDef()
.Param("alias", "DistortionPass")
.Param("filter_linear", "true")
.Param("scale", "1.0")
.Param("scale_type", "viewport"));
            TextureDefs.push_back(CrtShadersMame_hlslResourcesApertureGrilleTextureDef()
.Param("name", "MaskTexture"));
	}
};
}
