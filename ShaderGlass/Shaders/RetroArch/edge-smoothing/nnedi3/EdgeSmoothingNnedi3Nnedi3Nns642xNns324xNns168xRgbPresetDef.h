/*
ShaderGlass preset edge-smoothing-nnedi3 / nnedi3-nns64-2x-nns32-4x-nns16-8x-rgb imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/edge-smoothing/nnedi3/nnedi3-nns64-2x-nns32-4x-nns16-8x-rgb.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class EdgeSmoothingNnedi3Nnedi3Nns642xNns324xNns168xRgbPresetDef : public PresetDef
{
public:
	EdgeSmoothingNnedi3Nnedi3Nns642xNns324xNns168xRgbPresetDef() : PresetDef{}
	{
		Name = "nnedi3-nns64-2x-nns32-4x-nns16-8x-rgb";
		Category = "edge-smoothing-nnedi3";
	}

	virtual void Build() {
         	ShaderDefs.push_back(EdgeSmoothingNnedi3ShadersNnedi3Nns64Win8x4Pass1RgbShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "source")
.Param("scale_x", "1.0")
.Param("scale_y", "2.0"));
         	ShaderDefs.push_back(EdgeSmoothingNnedi3ShadersNnedi3Nns64Win8x4Pass2RgbShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "source")
.Param("scale_x", "2.0")
.Param("scale_y", "1.0"));
         	ShaderDefs.push_back(EdgeSmoothingNnedi3ShadersJinc2CshiftRgbShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "source")
.Param("scale_x", "1.0")
.Param("scale_y", "1.0"));
         	ShaderDefs.push_back(EdgeSmoothingNnedi3ShadersNnedi3Nns32Win8x4Pass1RgbShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "source")
.Param("scale_x", "1.0")
.Param("scale_y", "2.0"));
         	ShaderDefs.push_back(EdgeSmoothingNnedi3ShadersNnedi3Nns32Win8x4Pass2RgbShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "source")
.Param("scale_x", "2.0")
.Param("scale_y", "1.0"));
         	ShaderDefs.push_back(EdgeSmoothingNnedi3ShadersJinc2CshiftRgbShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "source")
.Param("scale_x", "1.0")
.Param("scale_y", "1.0"));
         	ShaderDefs.push_back(EdgeSmoothingNnedi3ShadersNnedi3Nns16Win8x4Pass1RgbShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "source")
.Param("scale_x", "1.0")
.Param("scale_y", "2.0"));
         	ShaderDefs.push_back(EdgeSmoothingNnedi3ShadersNnedi3Nns16Win8x4Pass2RgbShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "source")
.Param("scale_x", "2.0")
.Param("scale_y", "1.0"));
         	ShaderDefs.push_back(EdgeSmoothingNnedi3ShadersJinc2CshiftRgbShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "source")
.Param("scale_x", "1.0")
.Param("scale_y", "1.0"));
         	ShaderDefs.push_back(InterpolationShadersJinc2ShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "viewport"));
	}
};
}
