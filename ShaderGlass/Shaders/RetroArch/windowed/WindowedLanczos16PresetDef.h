/*
ShaderGlass preset windowed / lanczos16 imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/windowed/lanczos16.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class WindowedLanczos16PresetDef : public PresetDef
{
public:
	WindowedLanczos16PresetDef() : PresetDef{}
	{
		Name = "lanczos16";
		Category = "windowed";
	}

	virtual void Build() {
         	ShaderDefs.push_back(WindowedShadersLanczos16ShaderDef()
.Param("filter_linear", "false"));
	}
};
}
