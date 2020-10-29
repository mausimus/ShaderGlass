/*
ShaderGlass preset scalehq / 2xScaleHQ imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/scalehq/2xScaleHQ.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class Scalehq2xScaleHQPresetDef : public PresetDef
{
public:
	Scalehq2xScaleHQPresetDef() : PresetDef{}
	{
		Name = "2xScaleHQ";
		Category = "scalehq";
	}

	virtual void Build() {
         	ShaderDefs.push_back(ScalehqShaders2xScaleHQShaderDef()
.Param("filter_linear", "false")
.Param("scale", "2.0")
.Param("scale_type", "source"));
	}
};
}
