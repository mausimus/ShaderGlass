/*
ShaderGlass preset crt / crt-torridgristle imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/6f921ee4815a7894a33855974285b04545a4fa42/crt/crt-torridgristle.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class CrtCrtTorridgristlePresetDef : public PresetDef
{
public:
	CrtCrtTorridgristlePresetDef() : PresetDef{}
	{
		Name = "crt-torridgristle";
		Category = "crt";
	}

	virtual void Build() {
         	ShaderDefs.push_back(CrtShadersTorridgristleScanlineInterpolationShaderDef()
.Param("filter_linear", "true")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(CrtShadersTorridgristleScanlineSimpleShaderDef()
.Param("filter_linear", "true")
.Param("scale", "3.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(ReshadeShadersBlendoverlayBlendoverlayShaderDef()
.Param("filter_linear", "true")
.Param("scale_type", "viewport"));
         	ShaderDefs.push_back(CrtShadersTorridgristleBrightenShaderDef()
.Param("alias", "candy_ref")
.Param("filter_linear", "true"));
         	ShaderDefs.push_back(CrtShadersTorridgristleSunsetGaussianVertShaderDef()
.Param("filter_linear", "true")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(CrtShadersTorridgristleSunsetGaussianHorizShaderDef()
.Param("filter_linear", "true")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(CrtShadersTorridgristleCandyBloomShaderDef()
.Param("filter_linear", "true")
.Param("scale_type", "viewport"));
            TextureDefs.push_back(ReshadeShadersBlendoverlayShadowmask4x4TextureDef()
.Param("linear", "true")
.Param("name", "overlay"));
	}
};
}
