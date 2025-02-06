/*
ShaderGlass preset dithering / ps1-dedither-comparison imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/dithering/ps1-dedither-comparison.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class DitheringPs1DeditherComparisonPresetDef : public PresetDef
{
public:
	DitheringPs1DeditherComparisonPresetDef() : PresetDef{}
	{
		Name = "ps1-dedither-comparison";
		Category = "dithering";
	}

	virtual void Build() {
         	ShaderDefs.push_back(DitheringShadersPs1ditherPS1UnditherAntiBayerShaderDef()
.Param("filter_linear", "true")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(DitheringShadersPs1ditherPS1UnditherComparisonMkIIShaderDef()
.Param("filter_linear", "true")
.Param("scale", "1.0")
.Param("scale_type", "source"));
	}
};
}
