/*
ShaderGlass preset border-gameboy-player / gameboy-player-tvout-gba-color+interlacing imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/border/gameboy-player/gameboy-player-tvout-gba-color+interlacing.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class BorderGameboyPlayerGameboyPlayerTvoutGbaColorInterlacingPresetDef : public PresetDef
{
public:
	BorderGameboyPlayerGameboyPlayerTvoutGbaColorInterlacingPresetDef() : PresetDef{}
	{
		Name = "gameboy-player-tvout-gba-color+interlacing";
		Category = "border-gameboy-player";
	}

	virtual void Build() {
         	ShaderDefs.push_back(HandheldShadersColorGbaColorShaderDef()
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(BorderShadersImgborderGbpShaderDef()
.Param("scale_type_x", "absolute")
.Param("scale_type_y", "absolute")
.Param("scale_x", "608")
.Param("scale_y", "448"));
         	ShaderDefs.push_back(CrtShadersTvoutTweaksShaderDef()
.Param("scale_type_x", "viewport")
.Param("scale_type_y", "source")
.Param("scale_x", "1.000000")
.Param("scale_y", "1.000000"));
         	ShaderDefs.push_back(MiscShadersImageAdjustmentShaderDef());
         	ShaderDefs.push_back(MiscShadersInterlacingShaderDef()
.Param("scale_type_y", "source")
.Param("scale_y", "1.000000"));
         	ShaderDefs.push_back(AutoBoxBoxCenterShaderDef()
.Param("filter_linear", "false"));
            TextureDefs.push_back(BorderGameboyPlayerGameboyPlayerTextureDef()
.Param("name", "BORDER"));
	}
};
}
