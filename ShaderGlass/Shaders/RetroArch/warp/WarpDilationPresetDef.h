/*
ShaderGlass preset warp / dilation imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/warp/dilation.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class WarpDilationPresetDef : public PresetDef
{
public:
	WarpDilationPresetDef() : PresetDef{}
	{
		Name = "dilation";
		Category = "warp";
	}

	virtual void Build() {
         	ShaderDefs.push_back(WarpShadersDilationShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
	}
};
}
