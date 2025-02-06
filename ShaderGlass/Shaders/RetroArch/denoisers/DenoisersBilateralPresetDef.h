/*
ShaderGlass preset denoisers / bilateral imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/denoisers/bilateral.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class DenoisersBilateralPresetDef : public PresetDef
{
public:
	DenoisersBilateralPresetDef() : PresetDef{}
	{
		Name = "bilateral";
		Category = "denoisers";
	}

	virtual void Build() {
         	ShaderDefs.push_back(DenoisersShadersBilateralShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "source"));
	}
};
}
