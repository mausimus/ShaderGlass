/*
ShaderGlass preset handheld-color-mod / gbc-color imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/handheld/color-mod/gbc-color.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class HandheldColorModGbcColorPresetDef : public PresetDef
{
public:
	HandheldColorModGbcColorPresetDef() : PresetDef{}
	{
		Name = "gbc-color";
		Category = "handheld-color-mod";
	}

	virtual void Build() {
         	ShaderDefs.push_back(HandheldShadersColorLutGBCLUTShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(HandheldShadersColorGbcColorShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
            TextureDefs.push_back(HandheldShadersColorLutGbcGrey1TextureDef()
.Param("linear", "true")
.Param("name", "SamplerLUT1"));
            TextureDefs.push_back(HandheldShadersColorLutGbcGrey2TextureDef()
.Param("linear", "true")
.Param("name", "SamplerLUT2"));
            OverrideParam("LUT_selector_param", (float)2.000000);
	}
};
}
