/*
ShaderGlass preset downsample-drez / drez_ps2_x-1280_y-896 imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/downsample/drez/drez_ps2_x-1280_y-896.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class DownsampleDrezDrez_ps2_x1280_y896PresetDef : public PresetDef
{
public:
	DownsampleDrezDrez_ps2_x1280_y896PresetDef() : PresetDef{}
	{
		Name = "drez_ps2_x-1280_y-896";
		Category = "downsample-drez";
	}

	virtual void Build() {
         	ShaderDefs.push_back(DownsampleShadersDrezGSharp_resamplerShaderDef()
.Param("filter_linear", "false")
.Param("scale_type_x", "absolute")
.Param("scale_type_y", "absolute")
.Param("scale_x", "1280")
.Param("scale_y", "896"));
	}
};
}
