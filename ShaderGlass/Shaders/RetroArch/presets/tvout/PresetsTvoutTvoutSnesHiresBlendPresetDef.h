/*
ShaderGlass preset presets-tvout / tvout+snes-hires-blend imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/presets/tvout/tvout+snes-hires-blend.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class PresetsTvoutTvoutSnesHiresBlendPresetDef : public PresetDef
{
public:
	PresetsTvoutTvoutSnesHiresBlendPresetDef() : PresetDef{}
	{
		Name = "tvout+snes-hires-blend";
		Category = "presets-tvout";
	}

	virtual void Build() {
         	ShaderDefs.push_back(CrtShadersSnesHiresBlendShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(CrtShadersTvoutTweaksShaderDef()
.Param("scale_type_x", "viewport")
.Param("scale_type_y", "source")
.Param("scale_x", "1.000000")
.Param("scale_y", "1.000000"));
         	ShaderDefs.push_back(MiscImageAdjustmentShaderDef());
	}
};
}
