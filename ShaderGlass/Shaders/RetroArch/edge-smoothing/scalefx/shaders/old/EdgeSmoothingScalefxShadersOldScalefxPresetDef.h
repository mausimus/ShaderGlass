/*
ShaderGlass preset edge-smoothing-scalefx-shaders-old / scalefx imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/edge-smoothing/scalefx/shaders/old/scalefx.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class EdgeSmoothingScalefxShadersOldScalefxPresetDef : public PresetDef
{
public:
	EdgeSmoothingScalefxShadersOldScalefxPresetDef() : PresetDef{}
	{
		Name = "scalefx";
		Category = "edge-smoothing-scalefx-shaders-old";
	}

	virtual void Build() {
         	ShaderDefs.push_back(StockStockShaderDef()
.Param("alias", "scalefx_old_refpass"));
         	ShaderDefs.push_back(EdgeSmoothingScalefxShadersOldScalefxPass0ShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(EdgeSmoothingScalefxShadersOldScalefxPass1ShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(EdgeSmoothingScalefxShadersOldScalefxPass2ShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(EdgeSmoothingScalefxShadersOldScalefxPass3ShaderDef()
.Param("filter_linear", "false")
.Param("scale", "3.0")
.Param("scale_type", "source"));
	}
};
}
