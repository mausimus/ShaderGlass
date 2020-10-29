/*
ShaderGlass preset cubic / cubic imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/cubic/cubic.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class CubicCubicPresetDef : public PresetDef
{
public:
	CubicCubicPresetDef() : PresetDef{}
	{
		Name = "cubic";
		Category = "cubic";
	}

	virtual void Build() {
         	ShaderDefs.push_back(CubicShadersCubicShaderDef()
.Param("filter_linear", "false"));
	}
};
}
