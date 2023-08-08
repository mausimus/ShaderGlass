/*
ShaderGlass preset presets-gizmo-crt / gizmo-crt-n64 imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/6f921ee4815a7894a33855974285b04545a4fa42/presets/gizmo-crt/gizmo-crt-n64.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class PresetsGizmoCrtGizmoCrtN64PresetDef : public PresetDef
{
public:
	PresetsGizmoCrtGizmoCrtN64PresetDef() : PresetDef{}
	{
		Name = "gizmo-crt-n64";
		Category = "presets-gizmo-crt";
	}

	virtual void Build() {
         	ShaderDefs.push_back(CrtShadersGizmoCrtShaderDef()
.Param("filter_linear", "true")
.Param("scale_type", "viewport"));
	}
};
}
