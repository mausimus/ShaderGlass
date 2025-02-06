/*
ShaderGlass preset pixel-art-scaling / sharp-bilinear-2x-prescale imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/pixel-art-scaling/sharp-bilinear-2x-prescale.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class PixelArtScalingSharpBilinear2xPrescalePresetDef : public PresetDef
{
public:
	PixelArtScalingSharpBilinear2xPrescalePresetDef() : PresetDef{}
	{
		Name = "sharp-bilinear-2x-prescale";
		Category = "pixel-art-scaling";
	}

	virtual void Build() {
         	ShaderDefs.push_back(StockStockShaderDef()
.Param("filter_linear", "false")
.Param("scale", "2.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(StockStockShaderDef()
.Param("filter_linear", "true")
.Param("scale_type", "viewport"));
	}
};
}
