/*
ShaderGlass preset misc / color-mangler imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/misc/color-mangler.slangp
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
