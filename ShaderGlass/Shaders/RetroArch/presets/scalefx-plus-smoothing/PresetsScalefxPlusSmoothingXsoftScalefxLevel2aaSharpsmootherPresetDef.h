/*
ShaderGlass preset presets-scalefx-plus-smoothing / xsoft+scalefx-level2aa+sharpsmoother imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/presets/scalefx-plus-smoothing/xsoft+scalefx-level2aa+sharpsmoother.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class PresetsScalefxPlusSmoothingXsoftScalefxLevel2aaSharpsmootherPresetDef : public PresetDef
{
public:
	PresetsScalefxPlusSmoothingXsoftScalefxLevel2aaSharpsmootherPresetDef() : PresetDef{}
	{
		Name = "xsoft+scalefx-level2aa+sharpsmoother";
		Category = "presets-scalefx-plus-smoothing";
	}

	virtual void Build() {
         	ShaderDefs.push_back(InterpolationShadersJinc2ShaderDef()
.Param("alias", "refpass")
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(EdgeSmoothingScalefxShadersScalefxPass0ShaderDef()
.Param("alias", "scalefx_pass0")
.Param("filter_linear", "false")
.Param("float_framebuffer", "true")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(EdgeSmoothingScalefxShadersScalefxPass1ShaderDef()
.Param("filter_linear", "false")
.Param("float_framebuffer", "true")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(EdgeSmoothingScalefxShadersScalefxPass2ShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(EdgeSmoothingScalefxShadersScalefxPass3ShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(EdgeSmoothingScalefxShadersScalefxPass4ShaderDef()
.Param("filter_linear", "false")
.Param("scale", "3.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(BlursShadersSharpsmootherShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(StockStockShaderDef()
.Param("filter_linear", "false")
.Param("scale", "2.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(AntiAliasingShadersAaShader40Level2AaShader40Level2Pass1ShaderDef()
.Param("filter_linear", "true")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(AntiAliasingShadersAaShader40Level2AaShader40Level2Pass2ShaderDef()
.Param("filter_linear", "true")
.Param("scale", "1.0")
.Param("scale_type", "source"));
            OverrideParam("JINC2_AR_STRENGTH", (float)0.300000);
            OverrideParam("JINC2_SINC", (float)0.950000);
            OverrideParam("JINC2_WINDOW_SINC", (float)0.550000);
            OverrideParam("SFX_CLR", (float)0.600000);
            OverrideParam("SFX_SAA", (float)0.000000);
	}
};
}
