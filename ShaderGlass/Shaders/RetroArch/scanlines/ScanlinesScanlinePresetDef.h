/*
ShaderGlass preset scanlines / scanline imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/scanlines/scanline.slangp
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
         	ShaderDefs.push_back(ScanlinesShadersScanlineShaderDef()
.Param("scale_type", "viewport"));
	}
};
}
