/*
ShaderGlass preset pixel-art-scaling / cleanEdge-rotate imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/pixel-art-scaling/cleanEdge-rotate.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class PixelArtScalingCleanEdgeRotatePresetDef : public PresetDef
{
public:
	PixelArtScalingCleanEdgeRotatePresetDef() : PresetDef{}
	{
		Name = "cleanEdge-rotate";
		Category = "pixel-art-scaling";
	}

	virtual void Build() {
         	ShaderDefs.push_back(PixelArtScalingShadersCleanEdgeRotateShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
	}
};
}
