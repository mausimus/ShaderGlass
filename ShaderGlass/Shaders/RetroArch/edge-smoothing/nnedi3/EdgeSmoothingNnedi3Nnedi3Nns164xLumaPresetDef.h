/*
ShaderGlass preset edge-smoothing-nnedi3 / nnedi3-nns16-4x-luma imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/edge-smoothing/nnedi3/nnedi3-nns16-4x-luma.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class EdgeSmoothingNnedi3Nnedi3Nns164xLumaPresetDef : public PresetDef
{
public:
	EdgeSmoothingNnedi3Nnedi3Nns164xLumaPresetDef() : PresetDef{}
	{
		Name = "nnedi3-nns16-4x-luma";
		Category = "edge-smoothing-nnedi3";
	}

	virtual void Build() {
         	ShaderDefs.push_back(EdgeSmoothingNnedi3ShadersRgbToYuvShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "source")
.Param("scale_x", "1.0")
.Param("scale_y", "1.0"));
         	ShaderDefs.push_back(EdgeSmoothingNnedi3ShadersNnedi3Nns16Win8x4Pass1LumaShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "source")
.Param("scale_x", "1.0")
.Param("scale_y", "2.0"));
         	ShaderDefs.push_back(EdgeSmoothingNnedi3ShadersNnedi3Nns16Win8x4Pass2LumaShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "source")
.Param("scale_x", "2.0")
.Param("scale_y", "1.0"));
         	ShaderDefs.push_back(EdgeSmoothingNnedi3ShadersJinc2CshiftLumaShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "source")
.Param("scale_x", "1.0")
.Param("scale_y", "1.0"));
         	ShaderDefs.push_back(EdgeSmoothingNnedi3ShadersNnedi3Nns16Win8x4Pass1LumaShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "source")
.Param("scale_x", "1.0")
.Param("scale_y", "2.0"));
         	ShaderDefs.push_back(EdgeSmoothingNnedi3ShadersNnedi3Nns16Win8x4Pass2LumaShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "source")
.Param("scale_x", "2.0")
.Param("scale_y", "1.0"));
         	ShaderDefs.push_back(EdgeSmoothingNnedi3ShadersJinc2CshiftLumaShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "source")
.Param("scale_x", "1.0")
.Param("scale_y", "1.0"));
         	ShaderDefs.push_back(EdgeSmoothingNnedi3ShadersJinc2OnPassoutput0ShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "source")
.Param("scale_x", "1.0")
.Param("scale_y", "1.0"));
         	ShaderDefs.push_back(EdgeSmoothingNnedi3ShadersYuvToRgb4xShaderDef()
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
