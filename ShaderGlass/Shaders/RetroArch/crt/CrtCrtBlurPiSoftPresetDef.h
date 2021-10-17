/*
ShaderGlass preset crt / crt-blurPi-soft imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/crt/crt-blurPi-soft.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class CrtCrtBlurPiSoftPresetDef : public PresetDef
{
public:
	CrtCrtBlurPiSoftPresetDef() : PresetDef{}
	{
		Name = "crt-blurPi-soft";
		Category = "crt";
	}

	virtual void Build() {
         	ShaderDefs.push_back(CrtShadersCrtBlurPiShaderDef()
.Param("alias", "PASS0")
.Param("filter_linear", "true")
.Param("scale", "1.0")
.Param("scale_type", "viewport"));
	}
};
}
