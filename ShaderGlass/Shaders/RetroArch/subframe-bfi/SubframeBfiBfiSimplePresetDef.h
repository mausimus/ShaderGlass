/*
ShaderGlass preset subframe-bfi / bfi-simple imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/f1796f6f744c32da57b9d8c27ea1a20160128696/subframe-bfi/bfi-simple.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class SubframeBfiBfiSimplePresetDef : public PresetDef
{
public:
	SubframeBfiBfiSimplePresetDef() : PresetDef{}
	{
		Name = "bfi-simple";
		Category = "subframe-bfi";
	}

	void Build() {
         	ShaderDefs.push_back(SubframeBfiShadersBfiSimpleShaderDef());
	}
};
}
