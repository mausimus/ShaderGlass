/*
ShaderGlass preset handheld-console-border / gba-agb001-color-motionblur imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/handheld/console-border/gba-agb001-color-motionblur.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class HandheldConsoleBorderGbaAgb001ColorMotionblurPresetDef : public PresetDef
{
public:
	HandheldConsoleBorderGbaAgb001ColorMotionblurPresetDef() : PresetDef{}
	{
		Name = "gba-agb001-color-motionblur";
		Category = "handheld-console-border";
	}

	virtual void Build() {
         	ShaderDefs.push_back(MotionblurShadersResponseTimeShaderDef()
.Param("filter_linear", "false"));
         	ShaderDefs.push_back(HandheldShadersMgbaAgb001ShaderDef()
.Param("filter_linear", "false")
.Param("scale", "4.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(HandheldShadersColorGbaColorShaderDef()
.Param("filter_linear", "false"));
         	ShaderDefs.push_back(HandheldConsoleBorderShaderFilesBorderAutoScaleShaderDef()
.Param("filter_linear", "true"));
            TextureDefs.push_back(HandheldConsoleBorderResourcesGbaBorderSquare4xTextureDef()
.Param("linear", "true")
.Param("name", "BORDER"));
	}
};
}
