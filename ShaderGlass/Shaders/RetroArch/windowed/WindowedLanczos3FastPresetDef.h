/*
ShaderGlass preset windowed / lanczos3-fast imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/6f921ee4815a7894a33855974285b04545a4fa42/windowed/lanczos3-fast.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class WindowedLanczos3FastPresetDef : public PresetDef
{
public:
	WindowedLanczos3FastPresetDef() : PresetDef{}
	{
		Name = "lanczos3-fast";
		Category = "windowed";
	}

	virtual void Build() {
         	ShaderDefs.push_back(WindowedShadersLanczos3XShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type_x", "viewport")
.Param("scale_type_y", "source"));
         	ShaderDefs.push_back(WindowedShadersLanczos3YShaderDef()
.Param("filter_linear", "false"));
	}
};
}
