/*
ShaderGlass preset crt / crt-torridgristle imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/crt/crt-torridgristle.slangp
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
            OverrideParam("BrightenAmount", (float)0.100000);
            OverrideParam("BrightenLevel", (float)2.000000);
            OverrideParam("GlowLevel", (float)1.000000);
            OverrideParam("GlowTightness", (float)0.800000);
            OverrideParam("LUTHeight", (float)4.000000);
            OverrideParam("LUTWidth", (float)4.000000);
            OverrideParam("OverlayMix", (float)0.500000);
            OverrideParam("ScanlineSize", (float)3.000000);
            OverrideParam("YIQAmount", (float)0.350000);
	}
};
}
