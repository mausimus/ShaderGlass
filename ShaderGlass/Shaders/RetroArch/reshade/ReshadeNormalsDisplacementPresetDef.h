/*
ShaderGlass preset reshade / NormalsDisplacement imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/reshade/NormalsDisplacement.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class ReshadeNormalsDisplacementPresetDef : public PresetDef
{
public:
	ReshadeNormalsDisplacementPresetDef() : PresetDef{}
	{
		Name = "NormalsDisplacement";
		Category = "reshade";
	}

	virtual void Build() {
         	ShaderDefs.push_back(ReshadeShadersNormalsDisplacementNormalsDisplacementShaderDef());
            TextureDefs.push_back(ReshadeShadersNormalsDisplacementCurveNormalsTextureDef()
.Param("name", "displacementMap"));
	}
};
}
