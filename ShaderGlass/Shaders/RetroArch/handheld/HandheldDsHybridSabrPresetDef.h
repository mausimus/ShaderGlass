/*
ShaderGlass preset handheld / ds-hybrid-sabr imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/handheld/ds-hybrid-sabr.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class HandheldDsHybridSabrPresetDef : public PresetDef
{
public:
	HandheldDsHybridSabrPresetDef() : PresetDef{}
	{
		Name = "ds-hybrid-sabr";
		Category = "handheld";
	}

	virtual void Build() {
         	ShaderDefs.push_back(StockStockShaderDef()
.Param("alias", "ds_hybrid_view_refpass")
.Param("scale_type", "viewport"));
         	ShaderDefs.push_back(EdgeSmoothingSabrShadersSabrV30ShaderDef()
.Param("alias", "Ref")
.Param("filter_linear", "false")
.Param("scale", "2.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(HandheldShadersDsHybridViewShaderDef()
.Param("filter_linear", "true"));
	}
};
}
