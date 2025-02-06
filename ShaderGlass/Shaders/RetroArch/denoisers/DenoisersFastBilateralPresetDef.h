/*
ShaderGlass preset denoisers / fast-bilateral imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/denoisers/fast-bilateral.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class DenoisersFastBilateralPresetDef : public PresetDef
{
public:
	DenoisersFastBilateralPresetDef() : PresetDef{}
	{
		Name = "fast-bilateral";
		Category = "denoisers";
	}

	virtual void Build() {
         	ShaderDefs.push_back(DenoisersShadersFastBilateralShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "source"));
	}
};
}
