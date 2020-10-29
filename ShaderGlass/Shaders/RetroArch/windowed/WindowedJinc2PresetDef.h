/*
ShaderGlass preset windowed / jinc2 imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/windowed/jinc2.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class WindowedJinc2PresetDef : public PresetDef
{
public:
	WindowedJinc2PresetDef() : PresetDef{}
	{
		Name = "jinc2";
		Category = "windowed";
	}

	virtual void Build() {
         	ShaderDefs.push_back(WindowedShadersJinc2ShaderDef()
.Param("filter_linear", "false"));
	}
};
}
