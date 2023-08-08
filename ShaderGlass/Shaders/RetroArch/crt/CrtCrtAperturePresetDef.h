/*
ShaderGlass preset crt / crt-aperture imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/6f921ee4815a7894a33855974285b04545a4fa42/crt/crt-aperture.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class CrtCrtAperturePresetDef : public PresetDef
{
public:
	CrtCrtAperturePresetDef() : PresetDef{}
	{
		Name = "crt-aperture";
		Category = "crt";
	}

	virtual void Build() {
         	ShaderDefs.push_back(CrtShadersCrtApertureShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "viewport"));
	}
};
}
