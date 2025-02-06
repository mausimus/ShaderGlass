/*
ShaderGlass preset presets-gizmo-crt / gizmo-crt-snes-dark-curvator imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/presets/gizmo-crt/gizmo-crt-snes-dark-curvator.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class PresetsGizmoCrtGizmoCrtSnesDarkCurvatorPresetDef : public PresetDef
{
public:
	PresetsGizmoCrtGizmoCrtSnesDarkCurvatorPresetDef() : PresetDef{}
	{
		Name = "gizmo-crt-snes-dark-curvator";
		Category = "presets-gizmo-crt";
	}

	virtual void Build() {
         	ShaderDefs.push_back(CrtShadersGizmoCrtShaderDef()
.Param("filter_linear", "true")
.Param("scale_type", "viewport"));
	}
};
}
