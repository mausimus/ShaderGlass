/*
ShaderGlass preset motionblur / motionblur-blue imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/motionblur/motionblur-blue.slangp
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
