/*
ShaderGlass preset dithering / g-sharp_resampler imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/dithering/g-sharp_resampler.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class DitheringGSharp_resamplerPresetDef : public PresetDef
{
public:
	DitheringGSharp_resamplerPresetDef() : PresetDef{}
	{
		Name = "g-sharp_resampler";
		Category = "dithering";
	}

	virtual void Build() {
         	ShaderDefs.push_back(DitheringShadersGSharp_resamplerShaderDef()
.Param("scale", "1.0")
.Param("scale_type", "source"));
	}
};
}
