/*
ShaderGlass preset misc / simple_color_controls imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/misc/simple_color_controls.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class MiscSimple_color_controlsPresetDef : public PresetDef
{
public:
	MiscSimple_color_controlsPresetDef() : PresetDef{}
	{
		Name = "simple_color_controls";
		Category = "misc";
	}

	virtual void Build() {
         	ShaderDefs.push_back(MiscShadersSimple_color_controlsShaderDef()
.Param("scale_type", "source"));
	}
};
}
