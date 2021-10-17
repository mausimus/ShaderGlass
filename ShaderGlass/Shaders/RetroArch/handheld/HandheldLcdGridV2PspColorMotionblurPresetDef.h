/*
ShaderGlass preset handheld / lcd-grid-v2-psp-color-motionblur imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/handheld/lcd-grid-v2-psp-color-motionblur.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class HandheldLcdGridV2PspColorMotionblurPresetDef : public PresetDef
{
public:
	HandheldLcdGridV2PspColorMotionblurPresetDef() : PresetDef{}
	{
		Name = "lcd-grid-v2-psp-color-motionblur";
		Category = "handheld";
	}

	virtual void Build() {
         	ShaderDefs.push_back(MotionblurShadersResponseTimeShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(HandheldShadersLcdCgwgLcdGridV2ShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "viewport"));
         	ShaderDefs.push_back(HandheldShadersColorPspColorShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
	}
};
}
