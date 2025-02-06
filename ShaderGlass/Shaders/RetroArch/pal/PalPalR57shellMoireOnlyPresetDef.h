/*
ShaderGlass preset pal / pal-r57shell-moire-only imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/pal/pal-r57shell-moire-only.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class PalPalR57shellMoireOnlyPresetDef : public PresetDef
{
public:
	PalPalR57shellMoireOnlyPresetDef() : PresetDef{}
	{
		Name = "pal-r57shell-moire-only";
		Category = "pal";
	}

	virtual void Build() {
         	ShaderDefs.push_back(PalShadersPalR57shellMoireOnlyShaderDef()
.Param("scale_type", "source"));
	}
};
}
