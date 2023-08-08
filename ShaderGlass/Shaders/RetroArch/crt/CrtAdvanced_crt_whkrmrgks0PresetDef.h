/*
ShaderGlass preset crt / advanced_crt_whkrmrgks0 imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/6f921ee4815a7894a33855974285b04545a4fa42/crt/advanced_crt_whkrmrgks0.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class CrtAdvanced_crt_whkrmrgks0PresetDef : public PresetDef
{
public:
	CrtAdvanced_crt_whkrmrgks0PresetDef() : PresetDef{}
	{
		Name = "advanced_crt_whkrmrgks0";
		Category = "crt";
	}

	virtual void Build() {
         	ShaderDefs.push_back(CrtShadersAdvanced_CRT_shader_whkrmrgks0ShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "viewport"));
	}
};
}
