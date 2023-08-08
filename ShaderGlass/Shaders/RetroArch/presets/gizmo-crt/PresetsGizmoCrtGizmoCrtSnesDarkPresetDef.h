/*
ShaderGlass preset presets-gizmo-crt / gizmo-crt-snes-dark imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/6f921ee4815a7894a33855974285b04545a4fa42/presets/gizmo-crt/gizmo-crt-snes-dark.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class PresetsGizmoCrtGizmoCrtSnesDarkPresetDef : public PresetDef
{
public:
	PresetsGizmoCrtGizmoCrtSnesDarkPresetDef() : PresetDef{}
	{
		Name = "gizmo-crt-snes-dark";
		Category = "presets-gizmo-crt";
	}

	virtual void Build() {
         	ShaderDefs.push_back(CrtShadersGizmoCrtShaderDef()
.Param("filter_linear", "true")
.Param("scale_type", "viewport"));
	}
};
}
