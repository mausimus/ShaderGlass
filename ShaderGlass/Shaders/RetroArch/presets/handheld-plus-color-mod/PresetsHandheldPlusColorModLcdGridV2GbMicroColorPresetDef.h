/*
ShaderGlass preset presets-handheld-plus-color-mod / lcd-grid-v2-gbMicro-color imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/presets/handheld-plus-color-mod/lcd-grid-v2-gbMicro-color.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class PresetsHandheldPlusColorModLcdGridV2GbMicroColorPresetDef : public PresetDef
{
public:
	PresetsHandheldPlusColorModLcdGridV2GbMicroColorPresetDef() : PresetDef{}
	{
		Name = "lcd-grid-v2-gbMicro-color";
		Category = "presets-handheld-plus-color-mod";
	}

	virtual void Build() {
         	ShaderDefs.push_back(HandheldShadersColorLutGBMLUTShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(HandheldShadersLcdCgwgLcdGridV2ShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "viewport"));
         	ShaderDefs.push_back(HandheldShadersColorGbMicroColorShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
            TextureDefs.push_back(ReshadeShadersLUT64TextureDef()
.Param("linear", "true")
.Param("name", "SamplerLUT1"));
            TextureDefs.push_back(HandheldShadersColorLutGbamicroGreyTextureDef()
.Param("linear", "true")
.Param("name", "SamplerLUT2"));
	}
};
}
