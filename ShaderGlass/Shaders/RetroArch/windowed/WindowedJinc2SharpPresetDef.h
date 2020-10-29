/*
ShaderGlass preset windowed / jinc2-sharp imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/windowed/jinc2-sharp.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class WindowedJinc2SharpPresetDef : public PresetDef
{
public:
	WindowedJinc2SharpPresetDef() : PresetDef{}
	{
		Name = "jinc2-sharp";
		Category = "windowed";
	}

	virtual void Build() {
         	ShaderDefs.push_back(WindowedShadersJinc2ShaderDef()
.Param("filter_linear", "false"));
	}
};
}
