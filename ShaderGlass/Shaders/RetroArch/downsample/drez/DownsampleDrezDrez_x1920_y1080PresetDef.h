/*
ShaderGlass preset downsample-drez / drez_x-1920_y-1080 imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/downsample/drez/drez_x-1920_y-1080.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class DownsampleDrezDrez_x1920_y1080PresetDef : public PresetDef
{
public:
	DownsampleDrezDrez_x1920_y1080PresetDef() : PresetDef{}
	{
		Name = "drez_x-1920_y-1080";
		Category = "downsample-drez";
	}

	virtual void Build() {
         	ShaderDefs.push_back(DownsampleShadersDrezGSharp_resamplerShaderDef()
.Param("filter_linear", "false")
.Param("scale_type_x", "absolute")
.Param("scale_type_y", "absolute")
.Param("scale_x", "1920")
.Param("scale_y", "1080"));
	}
};
}
