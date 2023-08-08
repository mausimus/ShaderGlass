/*
ShaderGlass preset interpolation / half_res imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/6f921ee4815a7894a33855974285b04545a4fa42/interpolation/half_res.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class InterpolationHalf_resPresetDef : public PresetDef
{
public:
	InterpolationHalf_resPresetDef() : PresetDef{}
	{
		Name = "half_res";
		Category = "interpolation";
	}

	virtual void Build() {
         	ShaderDefs.push_back(StockStockShaderDef()
.Param("filter_linear", "false")
.Param("scale_type_x", "source")
.Param("scale_type_y", "source")
.Param("scale_x", "1.0")
.Param("scale_y", "0.5"));
	}
};
}
