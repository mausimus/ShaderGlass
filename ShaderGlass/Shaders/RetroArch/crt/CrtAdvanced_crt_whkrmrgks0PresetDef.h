/*
ShaderGlass preset crt / advanced_crt_whkrmrgks0 imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/crt/advanced_crt_whkrmrgks0.slangp
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
