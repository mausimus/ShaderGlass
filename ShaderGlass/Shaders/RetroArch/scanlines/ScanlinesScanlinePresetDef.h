/*
ShaderGlass preset scanlines / scanline imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/scanlines/scanline.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class ScanlinesScanlinePresetDef : public PresetDef
{
public:
	ScanlinesScanlinePresetDef() : PresetDef{}
	{
		Name = "scanline";
		Category = "scanlines";
	}

	virtual void Build() {
         	ShaderDefs.push_back(ScanlinesShadersScanlineShaderDef());
	}
};
}
