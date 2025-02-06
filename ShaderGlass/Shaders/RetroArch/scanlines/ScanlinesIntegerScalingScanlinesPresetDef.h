/*
ShaderGlass preset scanlines / integer-scaling-scanlines imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/scanlines/integer-scaling-scanlines.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class ScanlinesIntegerScalingScanlinesPresetDef : public PresetDef
{
public:
	ScanlinesIntegerScalingScanlinesPresetDef() : PresetDef{}
	{
		Name = "integer-scaling-scanlines";
		Category = "scanlines";
	}

	virtual void Build() {
         	ShaderDefs.push_back(ScanlinesShadersIntegerScalingScanlinesShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "viewport"));
	}
};
}
