/*
ShaderGlass preset cubic / cubic-gamma-correct imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/cubic/cubic-gamma-correct.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class CubicCubicGammaCorrectPresetDef : public PresetDef
{
public:
	CubicCubicGammaCorrectPresetDef() : PresetDef{}
	{
		Name = "cubic-gamma-correct";
		Category = "cubic";
	}

	virtual void Build() {
         	ShaderDefs.push_back(CubicShadersLinearizeShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source")
.Param("srgb_framebuffer", "true"));
         	ShaderDefs.push_back(CubicShadersCubicGammaCorrectShaderDef()
.Param("filter_linear", "false"));
	}
};
}
