/*
ShaderGlass preset crt / zfast-crt-curvature imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/6f921ee4815a7894a33855974285b04545a4fa42/crt/zfast-crt-curvature.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class CrtZfastCrtCurvaturePresetDef : public PresetDef
{
public:
	CrtZfastCrtCurvaturePresetDef() : PresetDef{}
	{
		Name = "zfast-crt-curvature";
		Category = "crt";
	}

	virtual void Build() {
         	ShaderDefs.push_back(CrtShadersZfast_crtZfast_crt_curvatureShaderDef()
.Param("filter_linear", "true")
.Param("scale_type", "viewport"));
	}
};
}
