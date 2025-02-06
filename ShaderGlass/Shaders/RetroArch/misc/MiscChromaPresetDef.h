/*
ShaderGlass preset misc / chroma imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/misc/chroma.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class MiscChromaPresetDef : public PresetDef
{
public:
	MiscChromaPresetDef() : PresetDef{}
	{
		Name = "chroma";
		Category = "misc";
	}

	virtual void Build() {
         	ShaderDefs.push_back(MiscShadersChromaShaderDef()
.Param("scale_type", "source"));
	}
};
}
