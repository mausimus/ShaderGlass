/*
ShaderGlass preset sharpen / adaptive-sharpen-multipass imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/sharpen/adaptive-sharpen-multipass.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class SharpenAdaptiveSharpenMultipassPresetDef : public PresetDef
{
public:
	SharpenAdaptiveSharpenMultipassPresetDef() : PresetDef{}
	{
		Name = "adaptive-sharpen-multipass";
		Category = "sharpen";
	}

	virtual void Build() {
         	ShaderDefs.push_back(SharpenShadersAdaptiveSharpenPass1ShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(SharpenShadersAdaptiveSharpenPass2ShaderDef()
.Param("filter_linear", "false"));
	}
};
}
