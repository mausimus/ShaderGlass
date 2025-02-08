/*
ShaderGlass preset pixel-art-scaling / uniform-nearest imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/pixel-art-scaling/uniform-nearest.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class PixelArtScalingUniformNearestPresetDef : public PresetDef
{
public:
	PixelArtScalingUniformNearestPresetDef() : PresetDef{}
	{
		Name = "uniform-nearest";
		Category = "pixel-art-scaling";
	}

	virtual void Build() {
         	ShaderDefs.push_back(PixelArtScalingShadersUniformNearestShaderDef()
.Param("filter_linear", "true")
.Param("scale_type", "viewport"));
            OverrideParam("BGR_LCD_PATTERN", (float)0.000000);
	}
};
}
