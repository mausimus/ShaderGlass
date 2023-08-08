/*
ShaderGlass preset misc / retro-palettes imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/6f921ee4815a7894a33855974285b04545a4fa42/misc/retro-palettes.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class MiscRetroPalettesPresetDef : public PresetDef
{
public:
	MiscRetroPalettesPresetDef() : PresetDef{}
	{
		Name = "retro-palettes";
		Category = "misc";
	}

	virtual void Build() {
         	ShaderDefs.push_back(MiscShadersRetroPalettesShaderDef()
.Param("scale_type", "source"));
	}
};
}
