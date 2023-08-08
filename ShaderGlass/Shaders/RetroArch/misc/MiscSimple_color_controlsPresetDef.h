/*
ShaderGlass preset misc / simple_color_controls imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/6f921ee4815a7894a33855974285b04545a4fa42/misc/simple_color_controls.slangp
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
