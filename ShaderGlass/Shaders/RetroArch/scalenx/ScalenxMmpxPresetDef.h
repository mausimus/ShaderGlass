/*
ShaderGlass preset scalenx / mmpx imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/scalenx/mmpx.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class ScalenxMmpxPresetDef : public PresetDef
{
public:
	ScalenxMmpxPresetDef() : PresetDef{}
	{
		Name = "mmpx";
		Category = "scalenx";
	}

	virtual void Build() {
         	ShaderDefs.push_back(ScalenxShadersMmpxShaderDef()
.Param("filter_linear", "false")
.Param("scale", "2.0")
.Param("scale_type", "source"));
	}
};
}
