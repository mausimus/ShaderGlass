/*
ShaderGlass preset crt / crt-blurPi-sharp imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/crt/crt-blurPi-sharp.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class CrtCrtBlurPiSharpPresetDef : public PresetDef
{
public:
	CrtCrtBlurPiSharpPresetDef() : PresetDef{}
	{
		Name = "crt-blurPi-sharp";
		Category = "crt";
	}

	virtual void Build() {
         	ShaderDefs.push_back(CrtShadersCrtBlurPiShaderDef()
.Param("alias", "PASS0")
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "viewport"));
	}
};
}
