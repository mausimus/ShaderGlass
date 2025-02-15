/*
ShaderGlass preset presets/handheld-plus-color-mod / lcd-grid-v2-dslite-color imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/presets/handheld-plus-color-mod/lcd-grid-v2-dslite-color.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class PresetsHandheldPlusColorModLcdGridV2DsliteColorPresetDef : public PresetDef
{
public:
	PresetsHandheldPlusColorModLcdGridV2DsliteColorPresetDef() : PresetDef{}
	{
		Name = "lcd-grid-v2-dslite-color";
		Category = "presets/handheld-plus-color-mod";
	}

	virtual void Build() {
         	ShaderDefs.push_back(ReshadeShadersLUTMultiLUTShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(HandheldShadersLcdCgwgLcdGridV2ShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "viewport"));
         	ShaderDefs.push_back(HandheldShadersColorDsliteColorShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
            TextureDefs.push_back(ReshadeShadersLUT64TextureDef()
.Param("linear", "true")
.Param("name", "SamplerLUT1"));
            TextureDefs.push_back(HandheldShadersColorLutDsliteGreyTextureDef()
.Param("linear", "true")
.Param("name", "SamplerLUT2"));
            OverrideParam("BGR", (float)1.000000);
            OverrideParam("BSUBPIX_B", (float)1.000000);
            OverrideParam("BSUBPIX_G", (float)0.000000);
            OverrideParam("BSUBPIX_R", (float)0.000000);
            OverrideParam("GSUBPIX_B", (float)0.000000);
            OverrideParam("GSUBPIX_G", (float)1.000000);
            OverrideParam("GSUBPIX_R", (float)0.000000);
            OverrideParam("RSUBPIX_B", (float)0.000000);
            OverrideParam("RSUBPIX_G", (float)0.000000);
            OverrideParam("RSUBPIX_R", (float)1.000000);
            OverrideParam("ambient", (float)0.000000);
            OverrideParam("blacklevel", (float)0.000000);
            OverrideParam("gain", (float)1.000000);
            OverrideParam("gamma", (float)2.200000);
	}
};
}
