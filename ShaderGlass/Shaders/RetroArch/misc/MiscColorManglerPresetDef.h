/*
ShaderGlass preset misc / color-mangler imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/6f921ee4815a7894a33855974285b04545a4fa42/misc/color-mangler.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class MiscColorManglerPresetDef : public PresetDef
{
public:
	MiscColorManglerPresetDef() : PresetDef{}
	{
		Name = "color-mangler";
		Category = "misc";
	}

	virtual void Build() {
         	ShaderDefs.push_back(MiscShadersColorManglerShaderDef()
.Param("scale_type", "source"));
	}
};
}
