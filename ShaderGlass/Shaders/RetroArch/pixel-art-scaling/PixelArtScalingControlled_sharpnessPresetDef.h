/*
ShaderGlass preset pixel-art-scaling / controlled_sharpness imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/pixel-art-scaling/controlled_sharpness.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class PixelArtScalingControlled_sharpnessPresetDef : public PresetDef
{
public:
	PixelArtScalingControlled_sharpnessPresetDef() : PresetDef{}
	{
		Name = "controlled_sharpness";
		Category = "pixel-art-scaling";
	}

	virtual void Build() {
         	ShaderDefs.push_back(PixelArtScalingShadersControlledSharpnessShaderDef()
.Param("filter_linear", "true")
.Param("scale_type", "viewport"));
	}
};
}
