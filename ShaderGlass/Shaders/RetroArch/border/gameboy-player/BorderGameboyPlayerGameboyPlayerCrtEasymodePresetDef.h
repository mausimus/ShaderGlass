/*
ShaderGlass preset border-gameboy-player / gameboy-player-crt-easymode imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/border/gameboy-player/gameboy-player-crt-easymode.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class BorderGameboyPlayerGameboyPlayerCrtEasymodePresetDef : public PresetDef
{
public:
	BorderGameboyPlayerGameboyPlayerCrtEasymodePresetDef() : PresetDef{}
	{
		Name = "gameboy-player-crt-easymode";
		Category = "border-gameboy-player";
	}

	virtual void Build() {
         	ShaderDefs.push_back(BorderShadersImgborderGbpShaderDef()
.Param("scale_type_x", "absolute")
.Param("scale_type_y", "absolute")
.Param("scale_x", "608")
.Param("scale_y", "448"));
         	ShaderDefs.push_back(CrtShadersCrtEasymodeShaderDef());
            TextureDefs.push_back(BorderGameboyPlayerGameboyPlayerTextureDef()
.Param("name", "BORDER"));
	}
};
}
