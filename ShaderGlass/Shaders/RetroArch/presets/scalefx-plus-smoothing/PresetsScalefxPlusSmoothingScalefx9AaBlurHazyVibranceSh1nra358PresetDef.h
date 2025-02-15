/*
ShaderGlass preset presets/scalefx-plus-smoothing / scalefx9-aa-blur-hazy-vibrance-sh1nra358 imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/presets/scalefx-plus-smoothing/scalefx9-aa-blur-hazy-vibrance-sh1nra358.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class PresetsScalefxPlusSmoothingScalefx9AaBlurHazyVibranceSh1nra358PresetDef : public PresetDef
{
public:
	PresetsScalefxPlusSmoothingScalefx9AaBlurHazyVibranceSh1nra358PresetDef() : PresetDef{}
	{
		Name = "scalefx9-aa-blur-hazy-vibrance-sh1nra358";
		Category = "presets/scalefx-plus-smoothing";
	}

	virtual void Build() {
         	ShaderDefs.push_back(StockStockShaderDef()
.Param("alias", "scalefx_old_refpass"));
         	ShaderDefs.push_back(EdgeSmoothingScalefxShadersOldScalefxPass0ShaderDef()
.Param("alias", "sfxp0")
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(EdgeSmoothingScalefxShadersOldScalefxPass1ShaderDef()
.Param("alias", "sfxp1")
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(EdgeSmoothingScalefxShadersOldScalefxPass2ShaderDef()
.Param("alias", "sfxp2")
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(EdgeSmoothingScalefxShadersOldScalefxPass3ShaderDef()
.Param("alias", "sfxp3")
.Param("filter_linear", "false")
.Param("scale", "3.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(EdgeSmoothingScalefxShadersOldScalefxPass0ShaderDef()
.Param("alias", "sfxp4")
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(EdgeSmoothingScalefxShadersOldScalefxPass1ShaderDef()
.Param("alias", "sfxp5")
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(EdgeSmoothingScalefxShadersOldScalefxPass2ShaderDef()
.Param("alias", "sfxp6")
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(EdgeSmoothingScalefxShadersOldScalefxPass7ShaderDef()
.Param("alias", "sfxp7")
.Param("filter_linear", "false")
.Param("scale", "3.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(MiscShadersImageAdjustmentShaderDef()
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
         	ShaderDefs.push_back(BlursShadersBlurGaussHShaderDef()
.Param("filter_linear", "true")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(BlursShadersBlurGaussVShaderDef()
.Param("filter_linear", "true")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(ReshadeShadersBlurHazeSh1nra358ShaderDef()
.Param("filter_linear", "true")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(ReshadeShadersVibrancePassSh1nra358ShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
            OverrideParam("INTERNAL_RES", (float)6.000000);
	}
};
}
