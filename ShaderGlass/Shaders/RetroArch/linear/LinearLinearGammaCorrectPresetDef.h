/*
ShaderGlass preset linear / linear-gamma-correct imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/linear/linear-gamma-correct.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class LinearLinearGammaCorrectPresetDef : public PresetDef
{
public:
	LinearLinearGammaCorrectPresetDef() : PresetDef{}
	{
		Name = "linear-gamma-correct";
		Category = "linear";
	}

	virtual void Build() {
         	ShaderDefs.push_back(LinearLinearizeShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source")
.Param("srgb_framebuffer", "true"));
         	ShaderDefs.push_back(LinearLinearGammaCorrectShaderDef()
.Param("filter_linear", "true"));
	}
};
}
