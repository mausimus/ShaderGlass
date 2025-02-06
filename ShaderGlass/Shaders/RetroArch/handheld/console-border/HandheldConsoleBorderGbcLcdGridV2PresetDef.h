/*
ShaderGlass preset handheld-console-border / gbc-lcd-grid-v2 imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/handheld/console-border/gbc-lcd-grid-v2.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class HandheldConsoleBorderGbcLcdGridV2PresetDef : public PresetDef
{
public:
	HandheldConsoleBorderGbcLcdGridV2PresetDef() : PresetDef{}
	{
		Name = "gbc-lcd-grid-v2";
		Category = "handheld-console-border";
	}

	virtual void Build() {
         	ShaderDefs.push_back(MotionblurShadersResponseTimeShaderDef()
.Param("filter_linear", "false"));
         	ShaderDefs.push_back(HandheldConsoleBorderShaderFilesLcdCgwgLcdGridV2ShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "viewport"));
         	ShaderDefs.push_back(HandheldShadersColorGbcColorShaderDef()
.Param("filter_linear", "false"));
         	ShaderDefs.push_back(HandheldConsoleBorderShaderFilesGbPass5ShaderDef()
.Param("filter_linear", "true"));
            TextureDefs.push_back(HandheldConsoleBorderResourcesColorBorderSquare4xTextureDef()
.Param("linear", "true")
.Param("name", "BORDER"));
	}
};
}
