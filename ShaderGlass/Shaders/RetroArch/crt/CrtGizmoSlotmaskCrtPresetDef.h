/*
ShaderGlass preset crt / gizmo-slotmask-crt imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/crt/gizmo-slotmask-crt.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class CrtGizmoSlotmaskCrtPresetDef : public PresetDef
{
public:
	CrtGizmoSlotmaskCrtPresetDef() : PresetDef{}
	{
		Name = "gizmo-slotmask-crt";
		Category = "crt";
	}

	virtual void Build() {
         	ShaderDefs.push_back(CrtShadersGizmoSlotmaskCrtShaderDef()
.Param("filter_linear", "true")
.Param("float_framebuffer", "false")
.Param("mipmap_input", "false")
.Param("srgb_framebuffer", "false"));
	}
};
}
