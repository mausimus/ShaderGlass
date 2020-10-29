/*
ShaderGlass preset scalehq / 4xScaleHQ imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/scalehq/4xScaleHQ.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class Scalehq4xScaleHQPresetDef : public PresetDef
{
public:
	Scalehq4xScaleHQPresetDef() : PresetDef{}
	{
		Name = "4xScaleHQ";
		Category = "scalehq";
	}

	virtual void Build() {
         	ShaderDefs.push_back(ScalehqShaders4xScaleHQShaderDef()
.Param("filter_linear", "false")
.Param("scale", "4.0")
.Param("scale_type", "source"));
	}
};
}
