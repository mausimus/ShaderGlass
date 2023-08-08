/*
ShaderGlass preset misc / image-adjustment imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/6f921ee4815a7894a33855974285b04545a4fa42/misc/image-adjustment.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class MiscImageAdjustmentPresetDef : public PresetDef
{
public:
	MiscImageAdjustmentPresetDef() : PresetDef{}
	{
		Name = "image-adjustment";
		Category = "misc";
	}

	virtual void Build() {
         	ShaderDefs.push_back(MiscShadersImageAdjustmentShaderDef()
.Param("scale_type", "source"));
	}
};
}
