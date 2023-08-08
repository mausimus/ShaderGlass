/*
ShaderGlass preset cubic / catmull-rom imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/6f921ee4815a7894a33855974285b04545a4fa42/cubic/catmull-rom.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class CubicCatmullRomPresetDef : public PresetDef
{
public:
	CubicCatmullRomPresetDef() : PresetDef{}
	{
		Name = "catmull-rom";
		Category = "cubic";
	}

	virtual void Build() {
         	ShaderDefs.push_back(CubicShadersCatmullRomShaderDef()
.Param("filter_linear", "true")
.Param("scale_type", "viewport"));
	}
};
}
