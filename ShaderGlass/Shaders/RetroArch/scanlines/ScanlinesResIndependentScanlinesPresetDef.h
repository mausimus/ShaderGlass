/*
ShaderGlass preset scanlines / res-independent-scanlines imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/scanlines/res-independent-scanlines.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class ScanlinesResIndependentScanlinesPresetDef : public PresetDef
{
public:
	ScanlinesResIndependentScanlinesPresetDef() : PresetDef{}
	{
		Name = "res-independent-scanlines";
		Category = "scanlines";
	}

	virtual void Build() {
         	ShaderDefs.push_back(ScanlinesShadersResIndependentScanlinesShaderDef()
.Param("scale_type", "viewport"));
	}
};
}
