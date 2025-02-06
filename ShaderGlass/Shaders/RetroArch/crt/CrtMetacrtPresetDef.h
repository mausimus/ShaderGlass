/*
ShaderGlass preset crt / metacrt imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/crt/metacrt.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class CrtMetacrtPresetDef : public PresetDef
{
public:
	CrtMetacrtPresetDef() : PresetDef{}
	{
		Name = "metacrt";
		Category = "crt";
	}

	virtual void Build() {
         	ShaderDefs.push_back(CrtShadersMetacrtBufCShaderDef()
.Param("filter_linear", "true")
.Param("float_framebuffer", "true")
.Param("scale_type", "viewport"));
         	ShaderDefs.push_back(CrtShadersMetacrtBufDShaderDef()
.Param("filter_linear", "true")
.Param("float_framebuffer", "true")
.Param("scale_type", "viewport"));
         	ShaderDefs.push_back(CrtShadersMetacrtImageShaderDef()
.Param("filter_linear", "true")
.Param("float_framebuffer", "true")
.Param("scale_type", "viewport"));
         	ShaderDefs.push_back(AntiAliasingShadersFxaaShaderDef()
.Param("filter_linear", "true")
.Param("scale_type", "viewport"));
            TextureDefs.push_back(CrtShadersMetacrtBasilicaTextureDef()
.Param("name", "cubeMap")
.Param("wrap_mode", "repeat"));
            TextureDefs.push_back(CrtShadersMetacrtWoodgrainTextureDef()
.Param("name", "table")
.Param("wrap_mode", "repeat"));
	}
};
}
