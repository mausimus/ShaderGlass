/*
ShaderGlass preset handheld-console-border / gg imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/handheld/console-border/gg.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class HandheldConsoleBorderGgPresetDef : public PresetDef
{
public:
	HandheldConsoleBorderGgPresetDef() : PresetDef{}
	{
		Name = "gg";
		Category = "handheld-console-border";
	}

	virtual void Build() {
         	ShaderDefs.push_back(HandheldConsoleBorderShaderFilesLcdCgwgLcdGridShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "viewport"));
         	ShaderDefs.push_back(HandheldConsoleBorderShaderFilesGbPass5ShaderDef()
.Param("filter_linear", "true"));
            TextureDefs.push_back(HandheldConsoleBorderResourcesGgBorderSquare4xTextureDef()
.Param("linear", "true")
.Param("mipmap", "false")
.Param("name", "BORDER")
.Param("wrap_mode", "clamp_to_border"));
            OverrideParam("GRID_STRENGTH", (float)0.050000);
            OverrideParam("PAR", (float)1.200000);
            OverrideParam("video_scale", (float)4.000000);
	}
};
}
