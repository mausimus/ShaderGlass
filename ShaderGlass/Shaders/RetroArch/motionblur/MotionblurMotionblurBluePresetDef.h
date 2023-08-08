/*
ShaderGlass preset motionblur / motionblur-blue imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/6f921ee4815a7894a33855974285b04545a4fa42/motionblur/motionblur-blue.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class MotionblurMotionblurBluePresetDef : public PresetDef
{
public:
	MotionblurMotionblurBluePresetDef() : PresetDef{}
	{
		Name = "motionblur-blue";
		Category = "motionblur";
	}

	virtual void Build() {
         	ShaderDefs.push_back(MotionblurShadersMotionblurBlueShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "source"));
	}
};
}
