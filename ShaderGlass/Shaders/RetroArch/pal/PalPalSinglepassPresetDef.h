/*
ShaderGlass preset pal / pal-singlepass imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/pal/pal-singlepass.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class PalPalSinglepassPresetDef : public PresetDef
{
public:
	PalPalSinglepassPresetDef() : PresetDef{}
	{
		Name = "pal-singlepass";
		Category = "pal";
	}

	virtual void Build() {
         	ShaderDefs.push_back(PalShadersPalSinglepassShaderDef());
	}
};
}
