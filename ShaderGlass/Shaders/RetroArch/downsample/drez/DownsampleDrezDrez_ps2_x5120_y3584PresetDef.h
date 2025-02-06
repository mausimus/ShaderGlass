/*
ShaderGlass preset downsample-drez / drez_ps2_x-5120_y-3584 imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/downsample/drez/drez_ps2_x-5120_y-3584.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class DownsampleDrezDrez_ps2_x5120_y3584PresetDef : public PresetDef
{
public:
	DownsampleDrezDrez_ps2_x5120_y3584PresetDef() : PresetDef{}
	{
		Name = "drez_ps2_x-5120_y-3584";
		Category = "downsample-drez";
	}

	virtual void Build() {
         	ShaderDefs.push_back(DownsampleShadersDrezGSharp_resamplerShaderDef()
.Param("filter_linear", "false")
.Param("scale_type_x", "absolute")
.Param("scale_type_y", "absolute")
.Param("scale_x", "5120")
.Param("scale_y", "3584"));
	}
};
}
