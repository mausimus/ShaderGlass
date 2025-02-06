/*
ShaderGlass preset reshade / NormalsDisplacement imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/reshade/NormalsDisplacement.slangp
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
         	ShaderDefs.push_back(ReshadeShadersNormalsDisplacementNormalsDisplacementShaderDef()
.Param("scale_type", "viewport"));
            TextureDefs.push_back(ReshadeShadersNormalsDisplacementCurveNormalsTextureDef()
.Param("name", "displacementMap"));
	}
};
}
