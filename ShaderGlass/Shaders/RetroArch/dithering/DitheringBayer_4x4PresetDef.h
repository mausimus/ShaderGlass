/*
ShaderGlass preset dithering / bayer_4x4 imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/dithering/bayer_4x4.slangp
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
            OverrideParam("COLOR_DEPTH", (float)2.000000);
            OverrideParam("DITHER_TUNE", (float)0.000000);
            OverrideParam("EGA_PALETTE", (float)0.000000);
	}
};
}
