/*
ShaderGlass preset subframe-bfi / crt-beam-simulator imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/f1796f6f744c32da57b9d8c27ea1a20160128696/subframe-bfi/crt-beam-simulator.slangp
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

	void Build() {
         	ShaderDefs.push_back(SubframeBfiShadersCrtBeamSimulatorShaderDef());
	}
};
}
