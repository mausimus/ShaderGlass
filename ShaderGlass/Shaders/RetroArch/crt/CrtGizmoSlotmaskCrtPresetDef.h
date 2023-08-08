/*
ShaderGlass preset crt / gizmo-slotmask-crt imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/6f921ee4815a7894a33855974285b04545a4fa42/crt/gizmo-slotmask-crt.slangp
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
