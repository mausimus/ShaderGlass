/*
ShaderGlass preset downsample / drez_1x imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/downsample/drez_1x.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class DownsampleDrez_1xPresetDef : public PresetDef
{
public:
	DownsampleDrez_1xPresetDef() : PresetDef{}
	{
		Name = "drez_1x";
		Category = "downsample";
	}

	virtual void Build() {
         	ShaderDefs.push_back(DownsampleShadersDrezGSharp_resamplerShaderDef()
.Param("filter_linear", "false")
.Param("scale_type_x", "source")
.Param("scale_type_y", "source")
.Param("scale_x", "1.0")
.Param("scale_y", "1.0"));
	}
};
}
