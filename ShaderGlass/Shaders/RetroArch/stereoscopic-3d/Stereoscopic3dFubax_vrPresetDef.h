/*
ShaderGlass preset stereoscopic-3d / fubax_vr imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/stereoscopic-3d/fubax_vr.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class Stereoscopic3dFubax_vrPresetDef : public PresetDef
{
public:
	Stereoscopic3dFubax_vrPresetDef() : PresetDef{}
	{
		Name = "fubax_vr";
		Category = "stereoscopic-3d";
	}

	virtual void Build() {
         	ShaderDefs.push_back(StockStockShaderDef()
.Param("scale", "1.0")
.Param("scale_type", "viewport"));
         	ShaderDefs.push_back(Stereoscopic3dShadersFubax_vrVRShaderDef()
.Param("filter_linear", "true")
.Param("scale", "1.0")
.Param("scale_type", "viewport"));
         	ShaderDefs.push_back(Stereoscopic3dShadersFubax_vrChromaticShaderDef()
.Param("filter_linear", "true")
.Param("scale", "1.0")
.Param("scale_type", "viewport"));
         	ShaderDefs.push_back(Stereoscopic3dShadersFubax_vrFilmicSharpenShaderDef()
.Param("filter_linear", "true")
.Param("scale", "1.0")
.Param("scale_type", "viewport"));
         	ShaderDefs.push_back(Stereoscopic3dShadersFubax_vrVR_noseShaderDef()
.Param("filter_linear", "true")
.Param("scale", "1.0")
.Param("scale_type", "viewport"));
            TextureDefs.push_back(Stereoscopic3dShadersFubax_vrNoseTextureDef()
.Param("name", "noseTex"));
	}
};
}
