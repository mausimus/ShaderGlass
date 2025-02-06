/*
ShaderGlass preset handheld-console-border / psp imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/handheld/console-border/psp.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class HandheldConsoleBorderPspPresetDef : public PresetDef
{
public:
	HandheldConsoleBorderPspPresetDef() : PresetDef{}
	{
		Name = "psp";
		Category = "handheld-console-border";
	}

	virtual void Build() {
         	ShaderDefs.push_back(HandheldConsoleBorderShaderFilesLcdCgwgLcdGridShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "viewport"));
         	ShaderDefs.push_back(HandheldConsoleBorderShaderFilesGbPass5ShaderDef()
.Param("filter_linear", "true"));
            TextureDefs.push_back(HandheldConsoleBorderResourcesPspBorderTextureDef()
.Param("linear", "true")
.Param("mipmap", "true")
.Param("name", "BORDER")
.Param("wrap_mode", "clamp_to_border"));
	}
};
}
