/*
ShaderGlass preset presets / my_old_tv imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/6f921ee4815a7894a33855974285b04545a4fa42/presets/my_old_tv.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class PresetsMy_old_tvPresetDef : public PresetDef
{
public:
	PresetsMy_old_tvPresetDef() : PresetDef{}
	{
		Name = "my_old_tv";
		Category = "presets";
	}

	virtual void Build() {
         	ShaderDefs.push_back(CrtShadersCrtConsumerShaderDef()
.Param("filter_linear", "true")
.Param("float_framebuffer", "false")
.Param("mipmap_input", "false")
.Param("scale_type_x", "viewport")
.Param("scale_type_y", "viewport")
.Param("scale_x", "1.000000")
.Param("scale_y", "1.000000")
.Param("srgb_framebuffer", "false"));
	}
};
}
