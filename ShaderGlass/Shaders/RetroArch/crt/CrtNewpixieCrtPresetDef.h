/*
ShaderGlass preset crt / newpixie-crt imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/6f921ee4815a7894a33855974285b04545a4fa42/crt/newpixie-crt.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class CrtNewpixieCrtPresetDef : public PresetDef
{
public:
	CrtNewpixieCrtPresetDef() : PresetDef{}
	{
		Name = "newpixie-crt";
		Category = "crt";
	}

	virtual void Build() {
         	ShaderDefs.push_back(CrtShadersNewpixieAccumulateShaderDef()
.Param("alias", "accum1")
.Param("filter_linear", "true")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(CrtShadersNewpixieBlur_horizShaderDef()
.Param("alias", "blur1")
.Param("filter_linear", "true")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(CrtShadersNewpixieBlur_vertShaderDef()
.Param("alias", "blur2")
.Param("filter_linear", "true")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(CrtShadersNewpixieNewpixieCrtShaderDef()
.Param("filter_linear", "true")
.Param("scale_type", "viewport"));
            TextureDefs.push_back(CrtShadersNewpixieCrtframeTextureDef()
.Param("linear", "true")
.Param("name", "frametexture")
.Param("wrap_mode", "clamp_to_border"));
	}
};
}
