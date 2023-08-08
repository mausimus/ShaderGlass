/*
ShaderGlass preset windowed / jinc2-sharper imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/6f921ee4815a7894a33855974285b04545a4fa42/windowed/jinc2-sharper.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class WindowedJinc2SharperPresetDef : public PresetDef
{
public:
	WindowedJinc2SharperPresetDef() : PresetDef{}
	{
		Name = "jinc2-sharper";
		Category = "windowed";
	}

	virtual void Build() {
         	ShaderDefs.push_back(WindowedShadersJinc2ShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "viewport"));
	}
};
}
