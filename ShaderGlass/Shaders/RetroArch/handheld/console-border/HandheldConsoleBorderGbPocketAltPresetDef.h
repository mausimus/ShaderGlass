/*
ShaderGlass preset handheld-console-border / gb-pocket-alt imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/handheld/console-border/gb-pocket-alt.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class HandheldConsoleBorderGbPocketAltPresetDef : public PresetDef
{
public:
	HandheldConsoleBorderGbPocketAltPresetDef() : PresetDef{}
	{
		Name = "gb-pocket-alt";
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
            TextureDefs.push_back(HandheldShadersGameboyResourcesSamplePalettesGbpPaletteTextureDef()
.Param("linear", "false")
.Param("name", "COLOR_PALETTE"));
            TextureDefs.push_back(HandheldShadersGameboyResourcesSampleBgsPaperBgTextureDef()
.Param("linear", "true")
.Param("name", "BACKGROUND"));
            TextureDefs.push_back(HandheldConsoleBorderResourcesPocketBorderSquare4xTextureDef()
.Param("linear", "true")
.Param("name", "BORDER"));
            OverrideParam("blending_mode", (float)0.000000);
            OverrideParam("grey_balance", (float)3.000000);
            OverrideParam("response_time", (float)0.444000);
            OverrideParam("video_scale", (float)4.000000);
	}
};
}
