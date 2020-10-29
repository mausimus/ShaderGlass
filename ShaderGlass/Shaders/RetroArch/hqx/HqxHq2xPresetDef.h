/*
ShaderGlass preset hqx / hq2x imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/hqx/hq2x.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class HqxHq2xPresetDef : public PresetDef
{
public:
	HqxHq2xPresetDef() : PresetDef{}
	{
		Name = "hq2x";
		Category = "hqx";
	}

	virtual void Build() {
         	ShaderDefs.push_back(HqxShaderFilesPass1ShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(HqxShaderFilesHq2xShaderDef()
.Param("filter_linear", "false")
.Param("scale", "2.0")
.Param("scale_type", "source"));
            TextureDefs.push_back(HqxResourcesHq2xTextureDef()
.Param("linear", "false")
.Param("name", "LUT"));
	}
};
}
