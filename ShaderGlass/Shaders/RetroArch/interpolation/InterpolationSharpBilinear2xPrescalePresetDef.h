/*
ShaderGlass preset interpolation / sharp-bilinear-2x-prescale imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/interpolation/sharp-bilinear-2x-prescale.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class InterpolationSharpBilinear2xPrescalePresetDef : public PresetDef
{
public:
	InterpolationSharpBilinear2xPrescalePresetDef() : PresetDef{}
	{
		Name = "sharp-bilinear-2x-prescale";
		Category = "interpolation";
	}

	virtual void Build() {
         	ShaderDefs.push_back(StockStockShaderDef()
.Param("filter_linear", "false")
.Param("scale", "2.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(StockStockShaderDef()
.Param("filter_linear", "true"));
	}
};
}
