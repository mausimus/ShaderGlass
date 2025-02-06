/*
ShaderGlass preset misc / half_res imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/misc/half_res.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class MiscHalf_resPresetDef : public PresetDef
{
public:
	MiscHalf_resPresetDef() : PresetDef{}
	{
		Name = "half_res";
		Category = "misc";
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
