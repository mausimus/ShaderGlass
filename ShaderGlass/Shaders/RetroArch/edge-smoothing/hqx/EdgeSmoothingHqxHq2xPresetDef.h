/*
ShaderGlass preset edge-smoothing-hqx / hq2x imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/edge-smoothing/hqx/hq2x.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class EdgeSmoothingHqxHq2xPresetDef : public PresetDef
{
public:
	EdgeSmoothingHqxHq2xPresetDef() : PresetDef{}
	{
		Name = "hq2x";
		Category = "edge-smoothing-hqx";
	}

	virtual void Build() {
         	ShaderDefs.push_back(StockStockShaderDef()
.Param("alias", "hqx_refpass"));
         	ShaderDefs.push_back(EdgeSmoothingHqxShadersPass1ShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(EdgeSmoothingHqxShadersHq2xShaderDef()
.Param("filter_linear", "false")
.Param("scale", "2.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(InterpolationShadersBicubicXShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type_x", "viewport")
.Param("scale_type_y", "source"));
         	ShaderDefs.push_back(InterpolationShadersBicubicYShaderDef()
.Param("filter_linear", "false"));
            TextureDefs.push_back(EdgeSmoothingHqxResourcesHq2xTextureDef()
.Param("linear", "false")
.Param("name", "LUT"));
	}
};
}
