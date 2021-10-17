/*
ShaderGlass preset handheld / lcd-shader imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/handheld/lcd-shader.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class HandheldLcdShaderPresetDef : public PresetDef
{
public:
	HandheldLcdShaderPresetDef() : PresetDef{}
	{
		Name = "lcd-shader";
		Category = "handheld";
	}

	virtual void Build() {
         	ShaderDefs.push_back(HandheldShadersLcdShaderLcdPass0ShaderDef()
.Param("alias", "PASS1")
.Param("filter_linear", "false")
.Param("scale", "1")
.Param("scale_type", "viewport"));
         	ShaderDefs.push_back(HandheldShadersLcdShaderLcdPass1ShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(HandheldShadersLcdShaderLcdPass2ShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(HandheldShadersLcdShaderLcdPass3ShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1")
.Param("scale_type", "source"));
            TextureDefs.push_back(HandheldShadersLcdShaderBackgroundTextureDef()
.Param("linear", "true")
.Param("name", "BACKGROUND"));
	}
};
}
