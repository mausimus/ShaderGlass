/*
ShaderGlass preset omniscale / omniscale-legacy imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/6f921ee4815a7894a33855974285b04545a4fa42/omniscale/omniscale-legacy.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class OmniscaleOmniscaleLegacyPresetDef : public PresetDef
{
public:
	OmniscaleOmniscaleLegacyPresetDef() : PresetDef{}
	{
		Name = "omniscale-legacy";
		Category = "omniscale";
	}

	virtual void Build() {
         	ShaderDefs.push_back(OmniscaleShadersOmniscaleLegacyShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "viewport"));
	}
};
}
