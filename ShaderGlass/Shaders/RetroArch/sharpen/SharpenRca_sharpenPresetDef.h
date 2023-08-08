/*
ShaderGlass preset sharpen / rca_sharpen imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/6f921ee4815a7894a33855974285b04545a4fa42/sharpen/rca_sharpen.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class SharpenRca_sharpenPresetDef : public PresetDef
{
public:
	SharpenRca_sharpenPresetDef() : PresetDef{}
	{
		Name = "rca_sharpen";
		Category = "sharpen";
	}

	virtual void Build() {
         	ShaderDefs.push_back(SharpenShadersRcasShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
	}
};
}
