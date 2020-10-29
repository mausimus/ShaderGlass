/*
ShaderGlass preset dithering / cbod_v1 imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/dithering/cbod_v1.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class DitheringCbod_v1PresetDef : public PresetDef
{
public:
	DitheringCbod_v1PresetDef() : PresetDef{}
	{
		Name = "cbod_v1";
		Category = "dithering";
	}

	virtual void Build() {
         	ShaderDefs.push_back(DitheringShadersCbodV1CbodV1Pass1ShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(DitheringShadersCbodV1CbodV1Pass2ShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
	}
};
}
