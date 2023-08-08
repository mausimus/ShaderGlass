/*
ShaderGlass preset dithering / blue_noise_dynamic_4Bit imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/6f921ee4815a7894a33855974285b04545a4fa42/dithering/blue_noise_dynamic_4Bit.slangp
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
	}
};
}
