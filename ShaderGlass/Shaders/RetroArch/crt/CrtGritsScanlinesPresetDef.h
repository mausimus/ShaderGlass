/*
ShaderGlass preset crt / GritsScanlines imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/6f921ee4815a7894a33855974285b04545a4fa42/crt/GritsScanlines.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class CrtGritsScanlinesPresetDef : public PresetDef
{
public:
	CrtGritsScanlinesPresetDef() : PresetDef{}
	{
		Name = "GritsScanlines";
		Category = "crt";
	}

	virtual void Build() {
         	ShaderDefs.push_back(CrtShadersGritsScanlinesGritsScanlinesShaderDef()
.Param("filter_linear", "true")
.Param("scale_type", "source")
.Param("scale_x", "1.0")
.Param("scale_y", "4.0"));
         	ShaderDefs.push_back(StockStockShaderDef()
.Param("filter_linear", "true")
.Param("scale_type", "viewport"));
            TextureDefs.push_back(CrtShadersGritsScanlinesScanlineLUT4pxTextureDef()
.Param("linear", "false")
.Param("name", "scanlines_LUT"));
            TextureDefs.push_back(CrtShadersGritsScanlinesTrinitron_colorTextureDef()
.Param("linear", "true")
.Param("name", "color_LUT"));
            TextureDefs.push_back(CrtShadersGritsScanlinesFakeSelfIlluminationTextureDef()
.Param("linear", "true")
.Param("name", "luminance_LUT"));
	}
};
}
