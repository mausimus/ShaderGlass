/*
ShaderGlass preset crt / crt-hyllian-sinc-curvature imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/6f921ee4815a7894a33855974285b04545a4fa42/crt/crt-hyllian-sinc-curvature.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class CrtCrtHyllianSincCurvaturePresetDef : public PresetDef
{
public:
	CrtCrtHyllianSincCurvaturePresetDef() : PresetDef{}
	{
		Name = "crt-hyllian-sinc-curvature";
		Category = "crt";
	}

	virtual void Build() {
         	ShaderDefs.push_back(CrtShadersHyllianCrtHyllianSincCurvatureShaderDef()
.Param("filter_linear", "false")
.Param("float_framebuffer", "true")
.Param("scale_type", "viewport"));
	}
};
}
