/*
ShaderGlass preset misc / colorimetry imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/misc/colorimetry.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class MiscColorimetryPresetDef : public PresetDef
{
public:
	MiscColorimetryPresetDef() : PresetDef{}
	{
		Name = "colorimetry";
		Category = "misc";
	}

	virtual void Build() {
         	ShaderDefs.push_back(MiscShadersColorimetryShaderDef()
.Param("scale_type", "source"));
	}
};
}
