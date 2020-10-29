/*
ShaderGlass preset hqx / hq3x imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/hqx/hq3x.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class HqxHq3xPresetDef : public PresetDef
{
public:
	HqxHq3xPresetDef() : PresetDef{}
	{
		Name = "hq3x";
		Category = "hqx";
	}

	virtual void Build() {
         	ShaderDefs.push_back(HqxShaderFilesPass1ShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(HqxShaderFilesHq3xShaderDef()
.Param("filter_linear", "false")
.Param("scale", "3.0")
.Param("scale_type", "source"));
            TextureDefs.push_back(HqxResourcesHq3xTextureDef()
.Param("linear", "false")
.Param("name", "LUT"));
	}
};
}
