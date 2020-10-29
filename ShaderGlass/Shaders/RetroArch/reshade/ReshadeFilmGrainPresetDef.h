/*
ShaderGlass preset reshade / FilmGrain imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/reshade/FilmGrain.slangp
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
