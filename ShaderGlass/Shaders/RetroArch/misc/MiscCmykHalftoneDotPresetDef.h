/*
ShaderGlass preset misc / cmyk-halftone-dot imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/6f921ee4815a7894a33855974285b04545a4fa42/misc/cmyk-halftone-dot.slangp
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
