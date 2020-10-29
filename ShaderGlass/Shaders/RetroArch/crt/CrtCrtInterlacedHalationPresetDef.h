/*
ShaderGlass preset crt / crt-interlaced-halation imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/crt/crt-interlaced-halation.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class CrtCrtInterlacedHalationPresetDef : public PresetDef
{
public:
	CrtCrtInterlacedHalationPresetDef() : PresetDef{}
	{
		Name = "crt-interlaced-halation";
		Category = "crt";
	}

	virtual void Build() {
         	ShaderDefs.push_back(CrtShadersCrtInterlacedHalationCrtInterlacedHalationPass0ShaderDef()
.Param("filter_linear", "false")
.Param("float_framebuffer", "false")
.Param("scale_type_x", "source")
.Param("scale_type_y", "source")
.Param("scale_x", "1.000000")
.Param("scale_y", "1.000000"));
         	ShaderDefs.push_back(CrtShadersCrtInterlacedHalationCrtInterlacedHalationPass1ShaderDef()
.Param("filter_linear", "false")
.Param("float_framebuffer", "false")
.Param("scale_type_x", "source")
.Param("scale_type_y", "source")
.Param("scale_x", "1.000000")
.Param("scale_y", "1.000000"));
         	ShaderDefs.push_back(CrtShadersCrtInterlacedHalationCrtInterlacedHalationPass2ShaderDef()
.Param("filter_linear", "false")
.Param("float_framebuffer", "false")
.Param("scale_type_x", "source")
.Param("scale_type_y", "source")
.Param("scale_x", "3.000000")
.Param("scale_y", "3.000000"));
         	ShaderDefs.push_back(StockStockShaderDef()
.Param("filter_linear", "true")
.Param("float_framebuffer", "false"));
	}
};
}
