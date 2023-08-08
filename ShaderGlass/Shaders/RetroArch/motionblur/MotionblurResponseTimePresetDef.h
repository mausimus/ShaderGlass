/*
ShaderGlass preset motionblur / response-time imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/6f921ee4815a7894a33855974285b04545a4fa42/motionblur/response-time.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class MotionblurResponseTimePresetDef : public PresetDef
{
public:
	MotionblurResponseTimePresetDef() : PresetDef{}
	{
		Name = "response-time";
		Category = "motionblur";
	}

	virtual void Build() {
         	ShaderDefs.push_back(MotionblurShadersResponseTimeShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "source"));
	}
};
}
