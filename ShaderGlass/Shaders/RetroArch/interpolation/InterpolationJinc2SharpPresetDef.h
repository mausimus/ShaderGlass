/*
ShaderGlass preset interpolation / jinc2-sharp imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/interpolation/jinc2-sharp.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class InterpolationJinc2SharpPresetDef : public PresetDef
{
public:
	InterpolationJinc2SharpPresetDef() : PresetDef{}
	{
		Name = "jinc2-sharp";
		Category = "interpolation";
	}

	virtual void Build() {
         	ShaderDefs.push_back(InterpolationShadersJinc2ShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "viewport"));
	}
};
}
