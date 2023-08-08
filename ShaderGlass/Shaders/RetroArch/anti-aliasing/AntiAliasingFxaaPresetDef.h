/*
ShaderGlass preset anti-aliasing / fxaa imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/6f921ee4815a7894a33855974285b04545a4fa42/anti-aliasing/fxaa.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class AntiAliasingFxaaPresetDef : public PresetDef
{
public:
	AntiAliasingFxaaPresetDef() : PresetDef{}
	{
		Name = "fxaa";
		Category = "anti-aliasing";
	}

	virtual void Build() {
         	ShaderDefs.push_back(AntiAliasingShadersFxaaShaderDef()
.Param("filter_linear", "true")
.Param("scale_type", "viewport"));
	}
};
}
