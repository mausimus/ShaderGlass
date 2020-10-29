/*
ShaderGlass preset reshade / bsnes-gamma-ramp imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/reshade/bsnes-gamma-ramp.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class ReshadeBsnesGammaRampPresetDef : public PresetDef
{
public:
	ReshadeBsnesGammaRampPresetDef() : PresetDef{}
	{
		Name = "bsnes-gamma-ramp";
		Category = "reshade";
	}

	virtual void Build() {
         	ShaderDefs.push_back(ReshadeShadersLUTLUTShaderDef()
.Param("filter_linear", "false")
.Param("scale_type_x", "source")
.Param("scale_type_y", "source")
.Param("scale_x", "1.000000")
.Param("scale_y", "1.000000"));
            TextureDefs.push_back(ReshadeShadersLUTBsnesTextureDef()
.Param("linear", "true")
.Param("name", "SamplerLUT"));
	}
};
}
