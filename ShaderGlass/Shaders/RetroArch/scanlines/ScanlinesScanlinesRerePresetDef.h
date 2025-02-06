/*
ShaderGlass preset scanlines / scanlines-rere imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/scanlines/scanlines-rere.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class ScanlinesScanlinesRerePresetDef : public PresetDef
{
public:
	ScanlinesScanlinesRerePresetDef() : PresetDef{}
	{
		Name = "scanlines-rere";
		Category = "scanlines";
	}

	virtual void Build() {
         	ShaderDefs.push_back(ScanlinesShadersScanlinesRereShaderDef()
.Param("filter_linear", "true")
.Param("scale_type", "viewport"));
	}
};
}
