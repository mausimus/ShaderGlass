/*
ShaderGlass preset omniscale / omniscale imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/6f921ee4815a7894a33855974285b04545a4fa42/omniscale/omniscale.slangp
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
.Param("scale_type", "viewport"));
	}
};
}
