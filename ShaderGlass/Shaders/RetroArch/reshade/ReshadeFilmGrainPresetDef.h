/*
ShaderGlass preset reshade / FilmGrain imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/reshade/FilmGrain.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class ReshadeFilmGrainPresetDef : public PresetDef
{
public:
	ReshadeFilmGrainPresetDef() : PresetDef{}
	{
		Name = "FilmGrain";
		Category = "reshade";
	}

	virtual void Build() {
         	ShaderDefs.push_back(ReshadeShadersFilmGrainShaderDef());
	}
};
}
