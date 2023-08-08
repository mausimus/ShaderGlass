/*
ShaderGlass preset interpolation / grid-blend-hybrid imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/6f921ee4815a7894a33855974285b04545a4fa42/interpolation/grid-blend-hybrid.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class InterpolationGridBlendHybridPresetDef : public PresetDef
{
public:
	InterpolationGridBlendHybridPresetDef() : PresetDef{}
	{
		Name = "grid-blend-hybrid";
		Category = "interpolation";
	}

	virtual void Build() {
         	ShaderDefs.push_back(InterpolationShadersGridBlendHybridShaderDef()
.Param("filter_linear", "true")
.Param("scale_type", "viewport"));
	}
};
}
