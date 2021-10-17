/*
ShaderGlass preset scanlines / scanline-fract imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/scanlines/scanline-fract.slangp
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
         	ShaderDefs.push_back(ScanlinesShadersScanlineFractShaderDef());
	}
};
}
