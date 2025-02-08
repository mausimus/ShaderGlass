/*
ShaderGlass preset anti-aliasing / smaa imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/anti-aliasing/smaa.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class AntiAliasingSmaaPresetDef : public PresetDef
{
public:
	AntiAliasingSmaaPresetDef() : PresetDef{}
	{
		Name = "smaa";
		Category = "anti-aliasing";
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
.Param("scale_type", "viewport"));
            TextureDefs.push_back(AntiAliasingShadersSmaaAreaTexTextureDef()
.Param("name", "areaTex"));
            TextureDefs.push_back(AntiAliasingShadersSmaaSearchTexTextureDef()
.Param("name", "searchTex"));
            OverrideParam("SMAA_CORNER_ROUNDING", (float)25.000000);
            OverrideParam("SMAA_EDT", (float)1.000000);
            OverrideParam("SMAA_LOCAL_CONTRAST_ADAPTATION_FACTOR", (float)2.000000);
            OverrideParam("SMAA_MAX_SEARCH_STEPS", (float)32.000000);
            OverrideParam("SMAA_MAX_SEARCH_STEPS_DIAG", (float)16.000000);
            OverrideParam("SMAA_THRESHOLD", (float)0.050000);
	}
};
}
