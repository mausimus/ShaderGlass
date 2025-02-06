/*
ShaderGlass preset pixel-art-scaling / smuberstep imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/pixel-art-scaling/smuberstep.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class PixelArtScalingSmuberstepPresetDef : public PresetDef
{
public:
	PixelArtScalingSmuberstepPresetDef() : PresetDef{}
	{
		Name = "smuberstep";
		Category = "pixel-art-scaling";
	}

	virtual void Build() {
         	ShaderDefs.push_back(PixelArtScalingShadersSmuberStepShaderDef()
.Param("filter_linear", "true")
.Param("scale_type", "viewport"));
	}
};
}
