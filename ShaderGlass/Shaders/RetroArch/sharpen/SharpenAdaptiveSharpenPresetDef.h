/*
ShaderGlass preset sharpen / adaptive-sharpen imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/6f921ee4815a7894a33855974285b04545a4fa42/sharpen/adaptive-sharpen.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class SharpenAdaptiveSharpenPresetDef : public PresetDef
{
public:
	SharpenAdaptiveSharpenPresetDef() : PresetDef{}
	{
		Name = "adaptive-sharpen";
		Category = "sharpen";
	}

	virtual void Build() {
         	ShaderDefs.push_back(SharpenShadersAdaptiveSharpenShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "source"));
	}
};
}
