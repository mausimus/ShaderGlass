/*
ShaderGlass preset handheld / sameboy-lcd-gbc-color-motionblur imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/handheld/sameboy-lcd-gbc-color-motionblur.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class HandheldSameboyLcdGbcColorMotionblurPresetDef : public PresetDef
{
public:
	HandheldSameboyLcdGbcColorMotionblurPresetDef() : PresetDef{}
	{
		Name = "sameboy-lcd-gbc-color-motionblur";
		Category = "handheld";
	}

	virtual void Build() {
         	ShaderDefs.push_back(MotionblurShadersResponseTimeShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(HandheldShadersSameboyLcdShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "viewport"));
         	ShaderDefs.push_back(HandheldShadersColorGbcColorShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "viewport"));
	}
};
}
