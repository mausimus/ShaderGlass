/*
ShaderGlass preset blurs / smart-blur imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/blurs/smart-blur.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class BlursSmartBlurPresetDef : public PresetDef
{
public:
	BlursSmartBlurPresetDef() : PresetDef{}
	{
		Name = "smart-blur";
		Category = "blurs";
	}

	virtual void Build() {
         	ShaderDefs.push_back(BlursShadersSmartBlurShaderDef());
	}
};
}
