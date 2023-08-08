/*
ShaderGlass preset hqx / hq2x imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/6f921ee4815a7894a33855974285b04545a4fa42/hqx/hq2x.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class HqxHq2xPresetDef : public PresetDef
{
public:
	HqxHq2xPresetDef() : PresetDef{}
	{
		Name = "hq2x";
		Category = "hqx";
	}

	virtual void Build() {
         	ShaderDefs.push_back(StockStockShaderDef()
.Param("alias", "hqx_refpass"));
         	ShaderDefs.push_back(HqxShadersPass1ShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(HqxShadersHq2xShaderDef()
.Param("filter_linear", "false")
.Param("scale", "2.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(CubicShadersBicubicXShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type_x", "viewport")
.Param("scale_type_y", "source"));
         	ShaderDefs.push_back(CubicShadersBicubicYShaderDef()
.Param("filter_linear", "false"));
            TextureDefs.push_back(HqxResourcesHq2xTextureDef()
.Param("linear", "false")
.Param("name", "LUT"));
	}
};
}
