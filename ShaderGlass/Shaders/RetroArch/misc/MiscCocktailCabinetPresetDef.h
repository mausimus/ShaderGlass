/*
ShaderGlass preset misc / cocktail-cabinet imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/misc/cocktail-cabinet.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class MiscCocktailCabinetPresetDef : public PresetDef
{
public:
	MiscCocktailCabinetPresetDef() : PresetDef{}
	{
		Name = "cocktail-cabinet";
		Category = "misc";
	}

	virtual void Build() {
         	ShaderDefs.push_back(MiscShadersCocktailCabinetShaderDef()
.Param("scale_type", "source"));
	}
};
}
