/*
ShaderGlass preset presets / scalefx9-aa-blur-hazy-ntsc-sh1nra358 imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/presets/scalefx9-aa-blur-hazy-ntsc-sh1nra358.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class PresetsScalefx9AaBlurHazyNtscSh1nra358PresetDef : public PresetDef
{
public:
	PresetsScalefx9AaBlurHazyNtscSh1nra358PresetDef() : PresetDef{}
	{
		Name = "scalefx9-aa-blur-hazy-ntsc-sh1nra358";
		Category = "presets";
	}

	virtual void Build() {
         	ShaderDefs.push_back(ScalefxShadersOldScalefxPass0ShaderDef()
.Param("alias", "sfxp0")
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(ScalefxShadersOldScalefxPass1ShaderDef()
.Param("alias", "sfxp1")
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(ScalefxShadersOldScalefxPass2ShaderDef()
.Param("alias", "sfxp2")
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(ScalefxShadersOldScalefxPass3ShaderDef()
.Param("alias", "sfxp3")
.Param("filter_linear", "false")
.Param("scale", "3.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(ScalefxShadersOldScalefxPass0ShaderDef()
.Param("alias", "sfxp4")
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(ScalefxShadersOldScalefxPass1ShaderDef()
.Param("alias", "sfxp5")
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(ScalefxShadersOldScalefxPass2ShaderDef()
.Param("alias", "sfxp6")
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(ScalefxShadersOldScalefxPass7ShaderDef()
.Param("alias", "sfxp7")
.Param("filter_linear", "false")
.Param("scale", "3.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(MiscImageAdjustmentShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(AntiAliasingShadersAaShader40ShaderDef()
.Param("filter_linear", "true")
.Param("scale", "1.0")
.Param("scale_type", "viewport"));
         	ShaderDefs.push_back(ReshadeShadersScanlinesShaderDef()
.Param("alias", "scanpass")
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(ReshadeShadersBloomPassSh1nra358ShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(BlursBlurGaussHShaderDef()
.Param("filter_linear", "true")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(BlursBlurGaussVShaderDef()
.Param("filter_linear", "true")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(ReshadeShadersBlurHazeSh1nra358ShaderDef()
.Param("filter_linear", "true")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(MiscNtscColorsShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
	}
};
}
