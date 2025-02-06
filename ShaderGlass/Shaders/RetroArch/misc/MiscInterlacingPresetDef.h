/*
ShaderGlass preset misc / interlacing imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/misc/interlacing.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class MiscInterlacingPresetDef : public PresetDef
{
public:
	MiscInterlacingPresetDef() : PresetDef{}
	{
		Name = "interlacing";
		Category = "misc";
	}

	virtual void Build() {
         	ShaderDefs.push_back(MiscShadersInterlacingShaderDef()
.Param("scale_type", "source"));
	}
};
}
