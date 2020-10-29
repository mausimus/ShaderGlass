/*
ShaderGlass preset omniscale / omniscale imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/omniscale/omniscale.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class OmniscaleOmniscalePresetDef : public PresetDef
{
public:
	OmniscaleOmniscalePresetDef() : PresetDef{}
	{
		Name = "omniscale";
		Category = "omniscale";
	}

	virtual void Build() {
         	ShaderDefs.push_back(OmniscaleShadersOmniscaleShaderDef()
.Param("filter_linear", "false")
.Param("scale", "10.0")
.Param("scale_type", "source"));
	}
};
}
