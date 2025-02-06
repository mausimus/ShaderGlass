/*
ShaderGlass preset misc / bob-deinterlacing imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/misc/bob-deinterlacing.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class MiscBobDeinterlacingPresetDef : public PresetDef
{
public:
	MiscBobDeinterlacingPresetDef() : PresetDef{}
	{
		Name = "bob-deinterlacing";
		Category = "misc";
	}

	virtual void Build() {
         	ShaderDefs.push_back(MiscShadersBobDeinterlacingShaderDef()
.Param("scale_type", "source"));
	}
};
}
