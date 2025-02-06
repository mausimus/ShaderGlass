/*
ShaderGlass preset border / ambient-glow imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/border/ambient-glow.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class BorderAmbientGlowPresetDef : public PresetDef
{
public:
	BorderAmbientGlowPresetDef() : PresetDef{}
	{
		Name = "ambient-glow";
		Category = "border";
	}

	virtual void Build() {
         	ShaderDefs.push_back(MotionblurShadersFeedbackShaderDef()
.Param("filter_linear", "true"));
         	ShaderDefs.push_back(BorderShadersAmbientGlowShaderDef()
.Param("filter_linear", "true")
.Param("mipmap_input", "true")
.Param("scale_type", "viewport"));
	}
};
}
