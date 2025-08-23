/*
ShaderGlass preset subframe-bfi / adaptive_strobe-koko imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/f1796f6f744c32da57b9d8c27ea1a20160128696/subframe-bfi/adaptive_strobe-koko.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class SubframeBfiAdaptive_strobeKokoPresetDef : public PresetDef
{
public:
	SubframeBfiAdaptive_strobeKokoPresetDef() : PresetDef{}
	{
		Name = "adaptive_strobe-koko";
		Category = "subframe-bfi";
	}

	void Build() {
         	ShaderDefs.push_back(SubframeBfiShadersAdaptive_strobeKokoShaderDef()
.Param("wrap_mode", "repeat"));
	}
};
}
