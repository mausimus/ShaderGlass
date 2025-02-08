/*
ShaderGlass preset anti-aliasing / smaa+linear imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/anti-aliasing/smaa+linear.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class AntiAliasingSmaaLinearPresetDef : public PresetDef
{
public:
	AntiAliasingSmaaLinearPresetDef() : PresetDef{}
	{
		Name = "smaa+linear";
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
.Param("scale_type", "source"));
         	ShaderDefs.push_back(StockStockShaderDef()
.Param("filter_linear", "true")
.Param("scale_type", "viewport"));
            TextureDefs.push_back(AntiAliasingShadersSmaaAreaTexTextureDef()
.Param("name", "areaTex"));
            TextureDefs.push_back(AntiAliasingShadersSmaaSearchTexTextureDef()
.Param("name", "searchTex"));
            OverrideParam("SMAA_EDT", (float)0.000000);
	}
};
}
