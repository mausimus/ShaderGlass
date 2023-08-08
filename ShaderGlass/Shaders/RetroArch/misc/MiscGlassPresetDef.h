/*
ShaderGlass preset misc / glass imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/6f921ee4815a7894a33855974285b04545a4fa42/misc/glass.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class MiscGlassPresetDef : public PresetDef
{
public:
	MiscGlassPresetDef() : PresetDef{}
	{
		Name = "glass";
		Category = "misc";
	}

	virtual void Build() {
         	ShaderDefs.push_back(MiscShadersGlassShaderDef()
.Param("scale_type", "viewport"));
	}
};
}
