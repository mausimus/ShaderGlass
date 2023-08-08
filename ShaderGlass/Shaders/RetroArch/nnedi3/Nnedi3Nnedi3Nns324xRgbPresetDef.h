/*
ShaderGlass preset nnedi3 / nnedi3-nns32-4x-rgb imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/6f921ee4815a7894a33855974285b04545a4fa42/nnedi3/nnedi3-nns32-4x-rgb.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class Nnedi3Nnedi3Nns324xRgbPresetDef : public PresetDef
{
public:
	Nnedi3Nnedi3Nns324xRgbPresetDef() : PresetDef{}
	{
		Name = "nnedi3-nns32-4x-rgb";
		Category = "nnedi3";
	}

	virtual void Build() {
         	ShaderDefs.push_back(Nnedi3ShadersNnedi3Nns32Win8x4Pass1RgbShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "source")
.Param("scale_x", "1.0")
.Param("scale_y", "2.0"));
         	ShaderDefs.push_back(Nnedi3ShadersNnedi3Nns32Win8x4Pass2RgbShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "source")
.Param("scale_x", "2.0")
.Param("scale_y", "1.0"));
         	ShaderDefs.push_back(Nnedi3ShadersJinc2CshiftRgbShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "source")
.Param("scale_x", "1.0")
.Param("scale_y", "1.0"));
         	ShaderDefs.push_back(Nnedi3ShadersNnedi3Nns32Win8x4Pass1RgbShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "source")
.Param("scale_x", "1.0")
.Param("scale_y", "2.0"));
         	ShaderDefs.push_back(Nnedi3ShadersNnedi3Nns32Win8x4Pass2RgbShaderDef()
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
.Param("filter_linear", "false")
.Param("scale_type", "viewport"));
	}
};
}
