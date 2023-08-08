/*
ShaderGlass preset dithering / bayer_4x4 imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/6f921ee4815a7894a33855974285b04545a4fa42/dithering/bayer_4x4.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class DitheringBayer_4x4PresetDef : public PresetDef
{
public:
	DitheringBayer_4x4PresetDef() : PresetDef{}
	{
		Name = "bayer_4x4";
		Category = "dithering";
	}

	virtual void Build() {
         	ShaderDefs.push_back(DitheringShadersBayer_4x4ShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "viewport"));
	}
};
}
