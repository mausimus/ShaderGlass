/*
ShaderGlass preset edge-smoothing-fsr / smaa+fsr imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/edge-smoothing/fsr/smaa+fsr.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class EdgeSmoothingFsrSmaaFsrPresetDef : public PresetDef
{
public:
	EdgeSmoothingFsrSmaaFsrPresetDef() : PresetDef{}
	{
		Name = "smaa+fsr";
		Category = "edge-smoothing-fsr";
	}

	virtual void Build() {
         	ShaderDefs.push_back(StockStockShaderDef()
.Param("alias", "SMAA_Input")
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(AntiAliasingShadersSmaaSmaaPass0ShaderDef()
.Param("filter_linear", "true")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(AntiAliasingShadersSmaaSmaaPass1ShaderDef()
.Param("filter_linear", "true")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(AntiAliasingShadersSmaaSmaaPass2ShaderDef()
.Param("filter_linear", "true")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(EdgeSmoothingFsrShadersFsrPass0ShaderDef()
.Param("filter_linear", "true")
.Param("scale", "1.0")
.Param("scale_type", "viewport")
.Param("wrap_mode", "clamp_to_edge"));
         	ShaderDefs.push_back(EdgeSmoothingFsrShadersFsrPass1ShaderDef()
.Param("filter_linear", "true")
.Param("scale", "1.0")
.Param("scale_type", "source")
.Param("wrap_mode", "clamp_to_edge"));
            TextureDefs.push_back(AntiAliasingShadersSmaaAreaTexTextureDef()
.Param("name", "areaTex"));
            TextureDefs.push_back(AntiAliasingShadersSmaaSearchTexTextureDef()
.Param("name", "searchTex"));
            OverrideParam("FSR_FILMGRAIN", (float)0.300000);
            OverrideParam("FSR_GRAINCOLOR", (float)1.000000);
            OverrideParam("FSR_GRAINPDF", (float)0.300000);
            OverrideParam("FSR_SHARPENING", (float)0.100000);
            OverrideParam("SMAA_CORNER_ROUNDING", (float)50.000000);
            OverrideParam("SMAA_EDT", (float)1.000000);
            OverrideParam("SMAA_LOCAL_CONTRAST_ADAPTATION_FACTOR", (float)2.500000);
            OverrideParam("SMAA_MAX_SEARCH_STEPS", (float)40.000000);
            OverrideParam("SMAA_MAX_SEARCH_STEPS_DIAG", (float)20.000000);
            OverrideParam("SMAA_THRESHOLD", (float)0.050000);
	}
};
}
