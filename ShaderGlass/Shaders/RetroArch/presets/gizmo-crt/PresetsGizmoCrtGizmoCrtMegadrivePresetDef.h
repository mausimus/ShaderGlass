/*
ShaderGlass preset presets-gizmo-crt / gizmo-crt-megadrive imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/presets/gizmo-crt/gizmo-crt-megadrive.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class PresetsGizmoCrtGizmoCrtMegadrivePresetDef : public PresetDef
{
public:
	PresetsGizmoCrtGizmoCrtMegadrivePresetDef() : PresetDef{}
	{
		Name = "gizmo-crt-megadrive";
		Category = "presets-gizmo-crt";
	}

	virtual void Build() {
         	ShaderDefs.push_back(CrtShadersGizmoCrtShaderDef()
.Param("filter_linear", "true")
.Param("scale_type", "viewport"));
            OverrideParam("BGR_LCD_PATTERN", (float)0.000000);
            OverrideParam("BLUR_OFFSET", (float)0.650000);
            OverrideParam("BRIGHTNESS", (float)0.300000);
            OverrideParam("CURVATURE_X", (float)0.000000);
            OverrideParam("CURVATURE_Y", (float)0.000000);
            OverrideParam("HORIZONTAL_BLUR", (float)1.000000);
            OverrideParam("SHRINK", (float)0.000000);
            OverrideParam("VERTICAL_BLUR", (float)0.000000);
	}
};
}
