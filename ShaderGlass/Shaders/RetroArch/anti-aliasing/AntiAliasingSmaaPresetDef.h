/*
ShaderGlass preset anti-aliasing / smaa imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/anti-aliasing/smaa.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class AntiAliasingSmaaPresetDef : public PresetDef
{
public:
	AntiAliasingSmaaPresetDef() : PresetDef{}
	{
		Name = "smaa";
		Category = "anti-aliasing";
	}

	virtual void Build() {
         	ShaderDefs.push_back(AntiAliasingShadersSmaaSmaaEdgeDetectionShaderDef()
.Param("filter_linear", "true")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(AntiAliasingShadersSmaaSmaaBlendWeightCalculationShaderDef()
.Param("filter_linear", "true")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(AntiAliasingShadersSmaaSmaaNeighborhoodBlendingShaderDef()
.Param("filter_linear", "true"));
            TextureDefs.push_back(AntiAliasingShadersSmaaAreaTexTextureDef()
.Param("name", "areaTex"));
            TextureDefs.push_back(AntiAliasingShadersSmaaSearchTexTextureDef()
.Param("name", "searchTex"));
	}
};
}
