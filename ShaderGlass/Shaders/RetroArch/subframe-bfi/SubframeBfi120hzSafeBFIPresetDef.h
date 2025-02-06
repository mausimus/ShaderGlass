/*
ShaderGlass preset subframe-bfi / 120hz-safe-BFI imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/subframe-bfi/120hz-safe-BFI.slangp
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

	virtual void Build() {
         	ShaderDefs.push_back(SubframeBfiShaders120hzSafeBFIShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
	}
};
}
