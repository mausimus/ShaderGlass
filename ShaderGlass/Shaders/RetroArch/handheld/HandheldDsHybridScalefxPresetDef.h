/*
ShaderGlass preset handheld / ds-hybrid-scalefx imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/handheld/ds-hybrid-scalefx.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class HandheldDsHybridScalefxPresetDef : public PresetDef
{
public:
	HandheldDsHybridScalefxPresetDef() : PresetDef{}
	{
		Name = "ds-hybrid-scalefx";
		Category = "handheld";
	}

	virtual void Build() {
         	ShaderDefs.push_back(StockStockShaderDef()
.Param("alias", "refpass")
.Param("filter_linear", "false")
.Param("scale_type", "viewport"));
         	ShaderDefs.push_back(EdgeSmoothingScalefxShadersScalefxPass0ShaderDef()
.Param("alias", "scalefx_pass0")
.Param("filter_linear", "false")
.Param("float_framebuffer", "true")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(EdgeSmoothingScalefxShadersScalefxPass1ShaderDef()
.Param("filter_linear", "false")
.Param("float_framebuffer", "true")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(EdgeSmoothingScalefxShadersScalefxPass2ShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(EdgeSmoothingScalefxShadersScalefxPass3ShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(EdgeSmoothingScalefxShadersScalefxPass4ShaderDef()
.Param("alias", "ds_hybrid_view_refpass")
.Param("filter_linear", "false")
.Param("scale", "3.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(HandheldShadersDsHybridViewShaderDef()
.Param("filter_linear", "true"));
	}
};
}
