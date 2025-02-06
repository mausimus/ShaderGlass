/*
ShaderGlass preset downsample-mixed-res-hooks / mixed-res-4x-append imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/downsample/mixed-res/hooks/mixed-res-4x-append.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class DownsampleMixedResHooksMixedRes4xAppendPresetDef : public PresetDef
{
public:
	DownsampleMixedResHooksMixedRes4xAppendPresetDef() : PresetDef{}
	{
		Name = "mixed-res-4x-append";
		Category = "downsample-mixed-res-hooks";
	}

	virtual void Build() {
         	ShaderDefs.push_back(StockStockShaderDef()
.Param("alias", "HiresSource"));
         	ShaderDefs.push_back(StockStockShaderDef()
.Param("alias", "BlurSource"));
         	ShaderDefs.push_back(DownsampleShadersMixedResOutputShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.000000")
.Param("scale_type", "source"));
	}
};
}
