/*
ShaderGlass preset blurs / gizmo-composite-blur imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/6f921ee4815a7894a33855974285b04545a4fa42/blurs/gizmo-composite-blur.slangp
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
