/*
ShaderGlass preset crt / crt-hyllian-curvature imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/6f921ee4815a7894a33855974285b04545a4fa42/crt/crt-hyllian-curvature.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class CrtCrtHyllianCurvaturePresetDef : public PresetDef
{
public:
	CrtCrtHyllianCurvaturePresetDef() : PresetDef{}
	{
		Name = "crt-hyllian-curvature";
		Category = "crt";
	}

	virtual void Build() {
         	ShaderDefs.push_back(CrtShadersHyllianCrtHyllianCurvatureShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "viewport"));
	}
};
}
