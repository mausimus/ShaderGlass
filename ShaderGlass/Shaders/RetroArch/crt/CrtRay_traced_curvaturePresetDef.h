/*
ShaderGlass preset crt / ray_traced_curvature imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/crt/ray_traced_curvature.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class CrtRay_traced_curvaturePresetDef : public PresetDef
{
public:
	CrtRay_traced_curvaturePresetDef() : PresetDef{}
	{
		Name = "ray_traced_curvature";
		Category = "crt";
	}

	virtual void Build() {
         	ShaderDefs.push_back(StockStockShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(CrtShadersRt_curvatureRt_curvatureShaderDef()
.Param("filter_linear", "true")
.Param("mipmap_input", "true")
.Param("scale_type", "viewport"));
	}
};
}
