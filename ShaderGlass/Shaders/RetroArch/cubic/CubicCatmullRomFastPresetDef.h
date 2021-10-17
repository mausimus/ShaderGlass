/*
ShaderGlass preset cubic / catmull-rom-fast imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/cubic/catmull-rom-fast.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class CubicCatmullRomFastPresetDef : public PresetDef
{
public:
	CubicCatmullRomFastPresetDef() : PresetDef{}
	{
		Name = "catmull-rom-fast";
		Category = "cubic";
	}

	virtual void Build() {
         	ShaderDefs.push_back(CubicShadersCatmullRomFastShaderDef()
.Param("filter_linear", "true"));
	}
};
}
