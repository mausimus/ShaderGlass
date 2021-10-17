/*
ShaderGlass preset crt / crt-hyllian-curvature imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/crt/crt-hyllian-curvature.slangp
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
.Param("filter_linear", "false"));
	}
};
}
