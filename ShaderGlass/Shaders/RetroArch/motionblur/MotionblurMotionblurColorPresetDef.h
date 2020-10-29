/*
ShaderGlass preset motionblur / motionblur-color imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/motionblur/motionblur-color.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class MotionblurMotionblurColorPresetDef : public PresetDef
{
public:
	MotionblurMotionblurColorPresetDef() : PresetDef{}
	{
		Name = "motionblur-color";
		Category = "motionblur";
	}

	virtual void Build() {
         	ShaderDefs.push_back(MotionblurShadersMotionblurColorShaderDef()
.Param("filter_linear", "false"));
	}
};
}
