/*
ShaderGlass preset crt / crt-hyllian-sinc imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/crt/crt-hyllian-sinc.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class CrtCrtHyllianSincPresetDef : public PresetDef
{
public:
	CrtCrtHyllianSincPresetDef() : PresetDef{}
	{
		Name = "crt-hyllian-sinc";
		Category = "crt";
	}

	virtual void Build() {
         	ShaderDefs.push_back(CrtShadersHyllianCrtHyllianSincShaderDef()
.Param("filter_linear", "false"));
	}
};
}
