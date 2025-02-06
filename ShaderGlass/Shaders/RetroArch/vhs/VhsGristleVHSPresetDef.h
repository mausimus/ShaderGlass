/*
ShaderGlass preset vhs / gristleVHS imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/vhs/gristleVHS.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class VhsGristleVHSPresetDef : public PresetDef
{
public:
	VhsGristleVHSPresetDef() : PresetDef{}
	{
		Name = "gristleVHS";
		Category = "vhs";
	}

	virtual void Build() {
         	ShaderDefs.push_back(StockStockShaderDef()
.Param("alias", "gristleVHS_refpass"));
         	ShaderDefs.push_back(VhsShadersGristleVHSHorizontalSoftenShaderDef()
.Param("filter_linear", "true")
.Param("frame_count_mod", "256")
.Param("scale_type", "absolute")
.Param("scale_x", "333")
.Param("scale_y", "480"));
         	ShaderDefs.push_back(BlursShadersSunset_gaussianSunset_horzShaderDef()
.Param("filter_linear", "true")
.Param("scale_type", "absolute")
.Param("scale_x", "80")
.Param("scale_y", "480"));
         	ShaderDefs.push_back(VhsShadersGristleVHSVHSShaderDef()
.Param("filter_linear", "true")
.Param("frame_count_mod", "256")
.Param("scale_type", "absolute")
.Param("scale_x", "333")
.Param("scale_y", "480"));
         	ShaderDefs.push_back(StockStockShaderDef()
.Param("filter_linear", "true")
.Param("scale_type", "viewport"));
	}
};
}
