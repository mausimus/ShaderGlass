/*
ShaderGlass preset crt / GritsScanlines imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/crt/GritsScanlines.slangp
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
