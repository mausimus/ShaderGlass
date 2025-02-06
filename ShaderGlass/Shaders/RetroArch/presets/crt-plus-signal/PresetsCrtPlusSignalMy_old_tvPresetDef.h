/*
ShaderGlass preset presets-crt-plus-signal / my_old_tv imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/presets/crt-plus-signal/my_old_tv.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class PresetsCrtPlusSignalMy_old_tvPresetDef : public PresetDef
{
public:
	PresetsCrtPlusSignalMy_old_tvPresetDef() : PresetDef{}
	{
		Name = "my_old_tv";
		Category = "presets-crt-plus-signal";
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
