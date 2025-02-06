/*
ShaderGlass preset motionblur / braid-rewind imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/motionblur/braid-rewind.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class MotionblurBraidRewindPresetDef : public PresetDef
{
public:
	MotionblurBraidRewindPresetDef() : PresetDef{}
	{
		Name = "braid-rewind";
		Category = "motionblur";
	}

	virtual void Build() {
         	ShaderDefs.push_back(MotionblurShadersBraidRewindShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "source"));
	}
};
}
