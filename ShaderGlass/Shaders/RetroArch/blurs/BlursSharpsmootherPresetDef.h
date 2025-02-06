/*
ShaderGlass preset blurs / sharpsmoother imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/blurs/sharpsmoother.slangp
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
