/*
ShaderGlass preset pixel-art-scaling / edge1pixel imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/pixel-art-scaling/edge1pixel.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class PixelArtScalingEdge1pixelPresetDef : public PresetDef
{
public:
	PixelArtScalingEdge1pixelPresetDef() : PresetDef{}
	{
		Name = "edge1pixel";
		Category = "pixel-art-scaling";
	}

	virtual void Build() {
         	ShaderDefs.push_back(PixelArtScalingShadersEdge1pixelShaderDef()
.Param("filter_linear", "true")
.Param("scale_type", "viewport"));
	}
};
}
