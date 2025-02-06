/*
ShaderGlass preset handheld / gameboy imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/handheld/gameboy.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class HandheldGameboyPresetDef : public PresetDef
{
public:
	HandheldGameboyPresetDef() : PresetDef{}
	{
		Name = "gameboy";
		Category = "handheld";
	}

	virtual void Build() {
         	ShaderDefs.push_back(HandheldShadersGameboyShaderFilesGbPass0ShaderDef()
.Param("alias", "PASS0")
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "viewport"));
         	ShaderDefs.push_back(HandheldShadersGameboyShaderFilesGbPass1ShaderDef()
.Param("alias", "PASS1")
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(HandheldShadersGameboyShaderFilesGbPass2ShaderDef()
.Param("alias", "PASS2")
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(HandheldShadersGameboyShaderFilesGbPass3ShaderDef()
.Param("alias", "PASS3")
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(HandheldShadersGameboyShaderFilesGbPass4ShaderDef()
.Param("alias", "PASS4")
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
            TextureDefs.push_back(HandheldShadersGameboyResourcesPaletteTextureDef()
.Param("linear", "false")
.Param("name", "COLOR_PALETTE"));
            TextureDefs.push_back(HandheldShadersGameboyResourcesBackgroundTextureDef()
.Param("linear", "true")
.Param("name", "BACKGROUND"));
	}
};
}
