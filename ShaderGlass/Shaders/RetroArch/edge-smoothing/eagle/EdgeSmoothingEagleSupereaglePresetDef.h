/*
ShaderGlass preset edge-smoothing-eagle / supereagle imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/edge-smoothing/eagle/supereagle.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class EdgeSmoothingEagleSupereaglePresetDef : public PresetDef
{
public:
	EdgeSmoothingEagleSupereaglePresetDef() : PresetDef{}
	{
		Name = "supereagle";
		Category = "edge-smoothing-eagle";
	}

	virtual void Build() {
         	ShaderDefs.push_back(EdgeSmoothingEagleShadersSupereagleShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "viewport"));
	}
};
}
