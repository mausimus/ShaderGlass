/*
ShaderGlass preset reshade / halftone-print imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/reshade/halftone-print.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class ReshadeHalftonePrintPresetDef : public PresetDef
{
public:
	ReshadeHalftonePrintPresetDef() : PresetDef{}
	{
		Name = "halftone-print";
		Category = "reshade";
	}

	virtual void Build() {
         	ShaderDefs.push_back(ReshadeShadersLUTLUTShaderDef()
.Param("scale_type", "viewport"));
         	ShaderDefs.push_back(MiscShadersCmykHalftoneDotShaderDef()
.Param("filter_linear", "true"));
            TextureDefs.push_back(ReshadeShadersLUTCmyk16TextureDef()
.Param("linear", "true")
.Param("name", "SamplerLUT"));
	}
};
}
