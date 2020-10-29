/*
ShaderGlass preset presets / c64-monitor imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/presets/c64-monitor.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class PresetsC64MonitorPresetDef : public PresetDef
{
public:
	PresetsC64MonitorPresetDef() : PresetDef{}
	{
		Name = "c64-monitor";
		Category = "presets";
	}

	virtual void Build() {
         	ShaderDefs.push_back(StockStockShaderDef()
.Param("scale_type_x", "absolute")
.Param("scale_x", "640.0"));
         	ShaderDefs.push_back(NtscShadersArtifactColorsArtifactColors0ShaderDef()
.Param("alias", "Pass1")
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(NtscShadersArtifactColorsArtifactColors1ShaderDef()
.Param("alias", "Pass2")
.Param("filter_linear", "false")
.Param("float_framebuffer", "true")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(NtscShadersArtifactColorsArtifactColors2ShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(CrtShadersCrtLottesShaderDef());
	}
};
}
