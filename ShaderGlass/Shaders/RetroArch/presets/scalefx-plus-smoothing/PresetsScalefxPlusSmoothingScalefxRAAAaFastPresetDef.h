/*
ShaderGlass preset presets-scalefx-plus-smoothing / scalefx+rAA+aa-fast imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/presets/scalefx-plus-smoothing/scalefx+rAA+aa-fast.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class PresetsScalefxPlusSmoothingScalefxRAAAaFastPresetDef : public PresetDef
{
public:
	PresetsScalefxPlusSmoothingScalefxRAAAaFastPresetDef() : PresetDef{}
	{
		Name = "scalefx+rAA+aa-fast";
		Category = "presets-scalefx-plus-smoothing";
	}

	virtual void Build() {
         	ShaderDefs.push_back(StockStockShaderDef()
.Param("alias", "refpass")
.Param("filter_linear", "false"));
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
         	ShaderDefs.push_back(AntiAliasingShadersReverseAaPost3xReverseAaPost3xPass0ShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(AntiAliasingShadersReverseAaPost3xReverseAaPost3xPass1ShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(AntiAliasingShadersFxaaShaderDef()
.Param("filter_linear", "true")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(AntiAliasingShadersAaShader40Level2AaShader40Level2Pass2ShaderDef()
.Param("filter_linear", "true")
.Param("scale", "2.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(DeblurShadersDeblurShaderDef()
.Param("filter_linear", "true")
.Param("scale", "1.0")
.Param("scale_type", "viewport"));
            OverrideParam("AAOFFSET2", (float)0.400000);
            OverrideParam("DEBLUR", (float)3.500000);
            OverrideParam("OFFSET", (float)2.000000);
            OverrideParam("SFX_CLR", (float)0.500000);
            OverrideParam("SFX_SAA", (float)0.000000);
            OverrideParam("SFX_SCN", (float)0.000000);
            OverrideParam("SMART", (float)0.400000);
	}
};
}
