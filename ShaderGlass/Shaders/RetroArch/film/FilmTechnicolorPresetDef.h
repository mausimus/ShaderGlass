/*
ShaderGlass preset film / technicolor imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/film/technicolor.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class FilmTechnicolorPresetDef : public PresetDef
{
public:
	FilmTechnicolorPresetDef() : PresetDef{}
	{
		Name = "technicolor";
		Category = "film";
	}

	virtual void Build() {
         	ShaderDefs.push_back(ReshadeShadersLUTLUTShaderDef());
         	ShaderDefs.push_back(FilmShadersFilm_noiseShaderDef());
            TextureDefs.push_back(ReshadeShadersLUTCmyk16TextureDef()
.Param("linear", "true")
.Param("name", "SamplerLUT"));
            TextureDefs.push_back(FilmResourcesFilm_noise1TextureDef()
.Param("linear", "true")
.Param("name", "noise1"));
	}
};
}
