/*
ShaderGlass preset crt / vector-glow-alt-render imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/crt/vector-glow-alt-render.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class CrtVectorGlowAltRenderPresetDef : public PresetDef
{
public:
	CrtVectorGlowAltRenderPresetDef() : PresetDef{}
	{
		Name = "vector-glow-alt-render";
		Category = "crt";
	}

	virtual void Build() {
         	ShaderDefs.push_back(MiscImageAdjustmentShaderDef()
.Param("alias", "PASS1"));
         	ShaderDefs.push_back(BlursBlur11fastVerticalShaderDef()
.Param("filter_linear", "true")
.Param("scale", "0.5")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(BlursBlur11fastHorizontalShaderDef()
.Param("filter_linear", "true")
.Param("mipmap_input", "true")
.Param("scale", "0.5")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(CrtShadersGlowTrailsCombineShaderDef()
.Param("scale", "1.0")
.Param("scale_type", "viewport"));
	}
};
}
