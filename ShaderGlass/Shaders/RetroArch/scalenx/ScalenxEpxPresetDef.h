/*
ShaderGlass preset scalenx / epx imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/scalenx/epx.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class ScalenxEpxPresetDef : public PresetDef
{
public:
	ScalenxEpxPresetDef() : PresetDef{}
	{
		Name = "epx";
		Category = "scalenx";
	}

	virtual void Build() {
         	ShaderDefs.push_back(ScalenxShadersEpxShaderDef()
.Param("filter_linear", "false")
.Param("scale", "2.0")
.Param("scale_type", "source"));
	}
};
}
