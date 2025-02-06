/*
ShaderGlass preset anti-aliasing / aa-shader-4.0 imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/anti-aliasing/aa-shader-4.0.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class AntiAliasingAaShader40PresetDef : public PresetDef
{
public:
	AntiAliasingAaShader40PresetDef() : PresetDef{}
	{
		Name = "aa-shader-4.0";
		Category = "anti-aliasing";
	}

	virtual void Build() {
         	ShaderDefs.push_back(AntiAliasingShadersAaShader40ShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "viewport"));
         	ShaderDefs.push_back(SharpenShadersAdaptiveSharpenShaderDef()
.Param("filter_linear", "false"));
	}
};
}
