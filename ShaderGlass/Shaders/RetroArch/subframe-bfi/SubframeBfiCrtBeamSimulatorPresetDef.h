/*
ShaderGlass preset subframe-bfi / crt-beam-simulator imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/subframe-bfi/crt-beam-simulator.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class SubframeBfiCrtBeamSimulatorPresetDef : public PresetDef
{
public:
	SubframeBfiCrtBeamSimulatorPresetDef() : PresetDef{}
	{
		Name = "crt-beam-simulator";
		Category = "subframe-bfi";
	}

	virtual void Build() {
         	ShaderDefs.push_back(SubframeBfiShadersCrtBeamSimulatorShaderDef());
	}
};
}
