/*
ShaderGlass preset blurs / sharpsmoother imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/6f921ee4815a7894a33855974285b04545a4fa42/blurs/sharpsmoother.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class BlursSharpsmootherPresetDef : public PresetDef
{
public:
	BlursSharpsmootherPresetDef() : PresetDef{}
	{
		Name = "sharpsmoother";
		Category = "blurs";
	}

	virtual void Build() {
         	ShaderDefs.push_back(BlursShadersSharpsmootherShaderDef());
	}
};
}
