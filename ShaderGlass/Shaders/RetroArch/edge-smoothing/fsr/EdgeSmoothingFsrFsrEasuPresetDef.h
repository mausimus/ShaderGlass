/*
ShaderGlass preset edge-smoothing-fsr / fsr-easu imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/edge-smoothing/fsr/fsr-easu.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class EdgeSmoothingFsrFsrEasuPresetDef : public PresetDef
{
public:
	EdgeSmoothingFsrFsrEasuPresetDef() : PresetDef{}
	{
		Name = "fsr-easu";
		Category = "edge-smoothing-fsr";
	}

	virtual void Build() {
         	ShaderDefs.push_back(EdgeSmoothingFsrShadersFsrPass0ShaderDef()
.Param("filter_linear", "true")
.Param("scale", "1.0")
.Param("scale_type", "viewport"));
	}
};
}
