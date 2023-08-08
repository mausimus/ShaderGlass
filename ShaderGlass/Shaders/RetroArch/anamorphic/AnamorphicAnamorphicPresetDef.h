/*
ShaderGlass preset anamorphic / anamorphic imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/6f921ee4815a7894a33855974285b04545a4fa42/anamorphic/anamorphic.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class AnamorphicAnamorphicPresetDef : public PresetDef
{
public:
	AnamorphicAnamorphicPresetDef() : PresetDef{}
	{
		Name = "anamorphic";
		Category = "anamorphic";
	}

	virtual void Build() {
         	ShaderDefs.push_back(AnamorphicShadersAnamorphicShaderDef()
.Param("filter_linear", "true"));
	}
};
}
