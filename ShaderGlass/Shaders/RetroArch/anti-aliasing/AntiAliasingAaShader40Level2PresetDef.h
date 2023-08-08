/*
ShaderGlass preset anti-aliasing / aa-shader-4.0-level2 imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/6f921ee4815a7894a33855974285b04545a4fa42/anti-aliasing/aa-shader-4.0-level2.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class AntiAliasingAaShader40Level2PresetDef : public PresetDef
{
public:
	AntiAliasingAaShader40Level2PresetDef() : PresetDef{}
	{
		Name = "aa-shader-4.0-level2";
		Category = "anti-aliasing";
	}

	virtual void Build() {
         	ShaderDefs.push_back(AntiAliasingShadersAaShader40Level2AaShader40Level2Pass1ShaderDef()
.Param("filter_linear", "false")
.Param("scale", "2.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(AntiAliasingShadersAaShader40Level2AaShader40Level2Pass2ShaderDef()
.Param("filter_linear", "false")
.Param("scale", "2.0")
.Param("scale_type", "source"));
	}
};
}
