/*
ShaderGlass preset dithering / blue_noise_dynamic_4Bit imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/dithering/blue_noise_dynamic_4Bit.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class DitheringBlue_noise_dynamic_4BitPresetDef : public PresetDef
{
public:
	DitheringBlue_noise_dynamic_4BitPresetDef() : PresetDef{}
	{
		Name = "blue_noise_dynamic_4Bit";
		Category = "dithering";
	}

	virtual void Build() {
         	ShaderDefs.push_back(DitheringShadersBlue_noise_dynamicShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "viewport"));
            TextureDefs.push_back(DitheringShadersBlueNoiseLDR_RGB1_0TextureDef()
.Param("name", "BlueNoiseTexture0"));
            OverrideParam("COLOR_DEPTH", (float)4.000000);
            OverrideParam("DITHER_TUNE", (float)0.000000);
            OverrideParam("EGA_PALETTE", (float)0.000000);
	}
};
}
