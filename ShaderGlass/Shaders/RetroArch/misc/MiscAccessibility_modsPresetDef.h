/*
ShaderGlass preset misc / accessibility_mods imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/misc/accessibility_mods.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class MiscAccessibility_modsPresetDef : public PresetDef
{
public:
	MiscAccessibility_modsPresetDef() : PresetDef{}
	{
		Name = "accessibility_mods";
		Category = "misc";
	}

	virtual void Build() {
         	ShaderDefs.push_back(MiscShadersAccessibility_modsShaderDef()
.Param("scale_type", "source"));
	}
};
}
