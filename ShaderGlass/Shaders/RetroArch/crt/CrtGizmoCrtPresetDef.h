/*
ShaderGlass preset crt / gizmo-crt imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/crt/gizmo-crt.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class CrtGizmoCrtPresetDef : public PresetDef
{
public:
	CrtGizmoCrtPresetDef() : PresetDef{}
	{
		Name = "gizmo-crt";
		Category = "crt";
	}

	virtual void Build() {
         	ShaderDefs.push_back(CrtShadersGizmoCrtShaderDef()
.Param("filter_linear", "true")
.Param("scale_type", "viewport"));
	}
};
}
