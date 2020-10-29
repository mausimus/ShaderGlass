/*
ShaderGlass preset nnedi3 / nnedi3-nns16-2x-rgb imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/nnedi3/nnedi3-nns16-2x-rgb.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class Nnedi3Nnedi3Nns162xRgbPresetDef : public PresetDef
{
public:
	Nnedi3Nnedi3Nns162xRgbPresetDef() : PresetDef{}
	{
		Name = "nnedi3-nns16-2x-rgb";
		Category = "nnedi3";
	}

	virtual void Build() {
         	ShaderDefs.push_back(Nnedi3ShadersNnedi3Nns16Win8x4Pass1RgbShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "source")
.Param("scale_x", "1.0")
.Param("scale_y", "2.0"));
         	ShaderDefs.push_back(Nnedi3ShadersNnedi3Nns16Win8x4Pass2RgbShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "source")
.Param("scale_x", "2.0")
.Param("scale_y", "1.0"));
         	ShaderDefs.push_back(Nnedi3ShadersJinc2CshiftRgbShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "source")
.Param("scale_x", "1.0")
.Param("scale_y", "1.0"));
         	ShaderDefs.push_back(WindowedShadersJinc2ShaderDef()
.Param("filter_linear", "false"));
	}
};
}
