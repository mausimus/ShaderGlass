/*
ShaderGlass preset presets-gizmo-crt / gizmo-crt-n64-curvator imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/presets/gizmo-crt/gizmo-crt-n64-curvator.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class PresetsGizmoCrtGizmoCrtN64CurvatorPresetDef : public PresetDef
{
public:
	PresetsGizmoCrtGizmoCrtN64CurvatorPresetDef() : PresetDef{}
	{
		Name = "gizmo-crt-n64-curvator";
		Category = "presets-gizmo-crt";
	}

	virtual void Build() {
         	ShaderDefs.push_back(CrtShadersGizmoCrtShaderDef()
.Param("filter_linear", "true")
.Param("scale_type", "viewport"));
	}
};
}
