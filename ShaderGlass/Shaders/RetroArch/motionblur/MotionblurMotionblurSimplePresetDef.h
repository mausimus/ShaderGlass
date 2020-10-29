/*
ShaderGlass preset motionblur / motionblur-simple imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/motionblur/motionblur-simple.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class MotionblurMotionblurSimplePresetDef : public PresetDef
{
public:
	MotionblurMotionblurSimplePresetDef() : PresetDef{}
	{
		Name = "motionblur-simple";
		Category = "motionblur";
	}

	virtual void Build() {
         	ShaderDefs.push_back(MotionblurShadersMotionblurSimpleShaderDef()
.Param("filter_linear", "false"));
	}
};
}
