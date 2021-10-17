/*
ShaderGlass preset fsr / fsr imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/fsr/fsr.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class FsrFsrPresetDef : public PresetDef
{
public:
	FsrFsrPresetDef() : PresetDef{}
	{
		Name = "fsr";
		Category = "fsr";
	}

	virtual void Build() {
         	ShaderDefs.push_back(FsrShadersFsrPass0ShaderDef()
.Param("filter_linear", "true")
.Param("scale", "1.0")
.Param("scale_type", "viewport"));
         	ShaderDefs.push_back(FsrShadersFsrPass1ShaderDef()
.Param("filter_linear", "true")
.Param("scale", "1.0")
.Param("scale_type", "source"));
	}
};
}
