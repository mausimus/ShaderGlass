/*
ShaderGlass preset crt / crt-hyllian-sinc imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/6f921ee4815a7894a33855974285b04545a4fa42/crt/crt-hyllian-sinc.slangp
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
.Param("filter_linear", "false")
.Param("scale_type", "viewport"));
	}
};
}
