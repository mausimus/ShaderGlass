/*
ShaderGlass preset crt / zfast-crt imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/crt/zfast-crt.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class CrtZfastCrtPresetDef : public PresetDef
{
public:
	CrtZfastCrtPresetDef() : PresetDef{}
	{
		Name = "zfast-crt";
		Category = "crt";
	}

	virtual void Build() {
         	ShaderDefs.push_back(CrtShadersZfast_crtShaderDef()
.Param("filter_linear", "true"));
	}
};
}
