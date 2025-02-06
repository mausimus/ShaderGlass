/*
ShaderGlass preset pixel-art-scaling / box_filter_aa_xform imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/pixel-art-scaling/box_filter_aa_xform.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class PixelArtScalingBox_filter_aa_xformPresetDef : public PresetDef
{
public:
	PixelArtScalingBox_filter_aa_xformPresetDef() : PresetDef{}
	{
		Name = "box_filter_aa_xform";
		Category = "pixel-art-scaling";
	}

	virtual void Build() {
         	ShaderDefs.push_back(PixelArtScalingShadersBox_filter_aaBox_filter_aa_xformShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "viewport"));
	}
};
}
