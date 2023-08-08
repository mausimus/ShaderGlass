/*
ShaderGlass preset scanlines / integer-scaling-scanlines imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/6f921ee4815a7894a33855974285b04545a4fa42/scanlines/integer-scaling-scanlines.slangp
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
