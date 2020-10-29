/*
ShaderGlass preset cubic / bicubic imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/cubic/bicubic.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class CubicBicubicPresetDef : public PresetDef
{
public:
	CubicBicubicPresetDef() : PresetDef{}
	{
		Name = "bicubic";
		Category = "cubic";
	}

	virtual void Build() {
         	ShaderDefs.push_back(CubicShadersBicubicShaderDef()
.Param("filter_linear", "false"));
	}
};
}
