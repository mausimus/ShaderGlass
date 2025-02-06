/*
ShaderGlass preset blurs / gizmo-composite-blur imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/blurs/gizmo-composite-blur.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class BlursGizmoCompositeBlurPresetDef : public PresetDef
{
public:
	BlursGizmoCompositeBlurPresetDef() : PresetDef{}
	{
		Name = "gizmo-composite-blur";
		Category = "blurs";
	}

	virtual void Build() {
         	ShaderDefs.push_back(BlursShadersGizmoBlurShaderDef()
.Param("filter_linear", "true")
.Param("scale_type", "viewport"));
	}
};
}
