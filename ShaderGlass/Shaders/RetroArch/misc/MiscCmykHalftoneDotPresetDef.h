/*
ShaderGlass preset misc / cmyk-halftone-dot imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/misc/cmyk-halftone-dot.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class MiscCmykHalftoneDotPresetDef : public PresetDef
{
public:
	MiscCmykHalftoneDotPresetDef() : PresetDef{}
	{
		Name = "cmyk-halftone-dot";
		Category = "misc";
	}

	virtual void Build() {
         	ShaderDefs.push_back(MiscShadersCmykHalftoneDotShaderDef()
.Param("scale_type", "viewport"));
	}
};
}
