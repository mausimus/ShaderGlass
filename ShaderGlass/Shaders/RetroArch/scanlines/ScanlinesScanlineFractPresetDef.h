/*
ShaderGlass preset scanlines / scanline-fract imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/6f921ee4815a7894a33855974285b04545a4fa42/scanlines/scanline-fract.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class ScanlinesScanlineFractPresetDef : public PresetDef
{
public:
	ScanlinesScanlineFractPresetDef() : PresetDef{}
	{
		Name = "scanline-fract";
		Category = "scanlines";
	}

	virtual void Build() {
         	ShaderDefs.push_back(ScanlinesShadersScanlineFractShaderDef()
.Param("scale_type", "viewport"));
	}
};
}
