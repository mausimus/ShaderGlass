/*
ShaderGlass preset denoisers / median_5x5 imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/denoisers/median_5x5.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class DenoisersMedian_5x5PresetDef : public PresetDef
{
public:
	DenoisersMedian_5x5PresetDef() : PresetDef{}
	{
		Name = "median_5x5";
		Category = "denoisers";
	}

	virtual void Build() {
         	ShaderDefs.push_back(DenoisersShadersMedian_5x5ShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
	}
};
}
