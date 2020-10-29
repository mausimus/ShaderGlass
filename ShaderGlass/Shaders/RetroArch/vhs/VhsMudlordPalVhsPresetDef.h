/*
ShaderGlass preset vhs / mudlord-pal-vhs imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/vhs/mudlord-pal-vhs.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class VhsMudlordPalVhsPresetDef : public PresetDef
{
public:
	VhsMudlordPalVhsPresetDef() : PresetDef{}
	{
		Name = "mudlord-pal-vhs";
		Category = "vhs";
	}

	virtual void Build() {
         	ShaderDefs.push_back(VhsShadersMudlordPalVhsShaderDef());
	}
};
}
