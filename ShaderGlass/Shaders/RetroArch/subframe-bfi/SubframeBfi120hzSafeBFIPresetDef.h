/*
ShaderGlass preset subframe-bfi / 120hz-safe-BFI imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/f1796f6f744c32da57b9d8c27ea1a20160128696/subframe-bfi/120hz-safe-BFI.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class SubframeBfi120hzSafeBFIPresetDef : public PresetDef
{
public:
	SubframeBfi120hzSafeBFIPresetDef() : PresetDef{}
	{
		Name = "120hz-safe-BFI";
		Category = "subframe-bfi";
	}

	void Build() {
         	ShaderDefs.push_back(SubframeBfiShaders120hzSafeBFIShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
	}
};
}
