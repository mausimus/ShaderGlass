/*
ShaderGlass preset hqx / hq4x imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/hqx/hq4x.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class HqxHq4xPresetDef : public PresetDef
{
public:
	HqxHq4xPresetDef() : PresetDef{}
	{
		Name = "hq4x";
		Category = "hqx";
	}

	virtual void Build() {
         	ShaderDefs.push_back(HqxShaderFilesPass1ShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(HqxShaderFilesHq4xShaderDef()
.Param("filter_linear", "false")
.Param("scale", "4.0")
.Param("scale_type", "source"));
            TextureDefs.push_back(HqxResourcesHq4xTextureDef()
.Param("linear", "false")
.Param("name", "LUT"));
	}
};
}
