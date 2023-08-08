/*
ShaderGlass preset fsr / fsr-easu imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/6f921ee4815a7894a33855974285b04545a4fa42/fsr/fsr-easu.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class FsrFsrEasuPresetDef : public PresetDef
{
public:
	FsrFsrEasuPresetDef() : PresetDef{}
	{
		Name = "fsr-easu";
		Category = "fsr";
	}

	virtual void Build() {
         	ShaderDefs.push_back(FsrShadersFsrPass0ShaderDef()
.Param("filter_linear", "true")
.Param("scale", "1.0")
.Param("scale_type", "viewport"));
	}
};
}
