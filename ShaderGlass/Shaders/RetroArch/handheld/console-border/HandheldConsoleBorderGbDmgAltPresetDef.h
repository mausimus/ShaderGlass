/*
ShaderGlass preset handheld-console-border / gb-dmg-alt imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/handheld/console-border/gb-dmg-alt.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class HandheldConsoleBorderGbDmgAltPresetDef : public PresetDef
{
public:
	HandheldConsoleBorderGbDmgAltPresetDef() : PresetDef{}
	{
		Name = "gb-dmg-alt";
		Category = "handheld-console-border";
	}

	virtual void Build() {
         	ShaderDefs.push_back(HandheldConsoleBorderShaderFilesGbPass0ShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1")
.Param("scale_type", "viewport"));
         	ShaderDefs.push_back(HandheldShadersGameboyShaderFilesGbPass1ShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(HandheldShadersGameboyShaderFilesGbPass2ShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(HandheldShadersGameboyShaderFilesGbPass3ShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(HandheldShadersGameboyShaderFilesGbPass4ShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(HandheldConsoleBorderShaderFilesGbPass5ShaderDef()
.Param("filter_linear", "true")
.Param("scale", "1")
.Param("scale_type", "source"));
            TextureDefs.push_back(HandheldShadersGameboyResourcesSamplePalettesDmgPalette0TextureDef()
.Param("linear", "false")
.Param("name", "COLOR_PALETTE"));
            TextureDefs.push_back(HandheldShadersGameboyResourcesSampleBgsDmgBgTextureDef()
.Param("linear", "true")
.Param("name", "BACKGROUND"));
            TextureDefs.push_back(HandheldConsoleBorderResourcesDmgBorderSquare4xTextureDef()
.Param("linear", "true")
.Param("name", "BORDER"));
	}
};
}
