/*
ShaderGlass preset scalefx-shaders-old / scalefx imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/6f921ee4815a7894a33855974285b04545a4fa42/scalefx/shaders/old/scalefx.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class ScalefxShadersOldScalefxPresetDef : public PresetDef
{
public:
	ScalefxShadersOldScalefxPresetDef() : PresetDef{}
	{
		Name = "scalefx";
		Category = "scalefx-shaders-old";
	}

	virtual void Build() {
         	ShaderDefs.push_back(StockStockShaderDef()
.Param("alias", "scalefx_old_refpass"));
         	ShaderDefs.push_back(ScalefxShadersOldScalefxPass0ShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(ScalefxShadersOldScalefxPass1ShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(ScalefxShadersOldScalefxPass2ShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(ScalefxShadersOldScalefxPass3ShaderDef()
.Param("filter_linear", "false")
.Param("scale", "3.0")
.Param("scale_type", "source"));
	}
};
}
