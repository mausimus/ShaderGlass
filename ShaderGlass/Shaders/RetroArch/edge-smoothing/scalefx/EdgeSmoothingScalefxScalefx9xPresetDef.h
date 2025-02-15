/*
ShaderGlass preset edge-smoothing/scalefx / scalefx-9x imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/edge-smoothing/scalefx/scalefx-9x.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class EdgeSmoothingScalefxScalefx9xPresetDef : public PresetDef
{
public:
	EdgeSmoothingScalefxScalefx9xPresetDef() : PresetDef{}
	{
		Name = "scalefx-9x";
		Category = "edge-smoothing/scalefx";
	}

	virtual void Build() {
         	ShaderDefs.push_back(StockStockShaderDef()
.Param("alias", "scalefx_old_refpass"));
         	ShaderDefs.push_back(EdgeSmoothingScalefxShadersOldScalefxPass0ShaderDef()
.Param("alias", "sfxp0")
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(EdgeSmoothingScalefxShadersOldScalefxPass1ShaderDef()
.Param("alias", "sfxp1")
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(EdgeSmoothingScalefxShadersOldScalefxPass2ShaderDef()
.Param("alias", "sfxp2")
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(EdgeSmoothingScalefxShadersOldScalefxPass3ShaderDef()
.Param("alias", "sfxp3")
.Param("filter_linear", "false")
.Param("scale", "3.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(EdgeSmoothingScalefxShadersOldScalefxPass0ShaderDef()
.Param("alias", "sfxp4")
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(EdgeSmoothingScalefxShadersOldScalefxPass1ShaderDef()
.Param("alias", "sfxp5")
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(EdgeSmoothingScalefxShadersOldScalefxPass2ShaderDef()
.Param("alias", "sfxp6")
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(EdgeSmoothingScalefxShadersOldScalefxPass7ShaderDef()
.Param("alias", "sfxp7")
.Param("filter_linear", "false")
.Param("scale", "3.0")
.Param("scale_type", "source"));
	}
};
}
