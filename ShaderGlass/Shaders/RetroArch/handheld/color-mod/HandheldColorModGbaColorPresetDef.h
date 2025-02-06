/*
ShaderGlass preset handheld-color-mod / gba-color imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/handheld/color-mod/gba-color.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class HandheldColorModGbaColorPresetDef : public PresetDef
{
public:
	HandheldColorModGbaColorPresetDef() : PresetDef{}
	{
		Name = "gba-color";
		Category = "handheld-color-mod";
	}

	virtual void Build() {
         	ShaderDefs.push_back(HandheldShadersColorLutGBALUTShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(HandheldShadersColorGbaColorShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
            TextureDefs.push_back(HandheldShadersColorLutGbaGrey1TextureDef()
.Param("linear", "true")
.Param("name", "SamplerLUT1"));
            TextureDefs.push_back(HandheldShadersColorLutGbaGrey2TextureDef()
.Param("linear", "true")
.Param("name", "SamplerLUT2"));
	}
};
}
