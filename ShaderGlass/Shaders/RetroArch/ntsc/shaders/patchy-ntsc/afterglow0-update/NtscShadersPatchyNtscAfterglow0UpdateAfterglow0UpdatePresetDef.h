/*
ShaderGlass preset ntsc-shaders-patchy-ntsc-afterglow0-update / afterglow0-update imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/ntsc/shaders/patchy-ntsc/afterglow0-update/afterglow0-update.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class NtscShadersPatchyNtscAfterglow0UpdateAfterglow0UpdatePresetDef : public PresetDef
{
public:
	NtscShadersPatchyNtscAfterglow0UpdateAfterglow0UpdatePresetDef() : PresetDef{}
	{
		Name = "afterglow0-update";
		Category = "ntsc-shaders-patchy-ntsc-afterglow0-update";
	}

	virtual void Build() {
         	ShaderDefs.push_back(NtscShadersPatchyNtscAfterglow0UpdateAfterglow0UpdatePass1ShaderDef()
.Param("filter_linear", "false")
.Param("float_framebuffer", "false")
.Param("mipmap_input", "false")
.Param("scale_type_x", "source")
.Param("scale_type_y", "source")
.Param("scale_x", "1.000000")
.Param("scale_y", "1.000000")
.Param("srgb_framebuffer", "false"));
         	ShaderDefs.push_back(NtscShadersPatchyNtscAfterglow0UpdateAfterglow0UpdatePass2ShaderDef()
.Param("filter_linear", "false")
.Param("float_framebuffer", "false")
.Param("mipmap_input", "false")
.Param("scale_type_x", "source")
.Param("scale_type_y", "source")
.Param("scale_x", "1.000000")
.Param("scale_y", "1.000000")
.Param("srgb_framebuffer", "false"));
         	ShaderDefs.push_back(NtscShadersPatchyNtscAfterglow0UpdateAfterglow0UpdatePass3ShaderDef()
.Param("filter_linear", "false")
.Param("float_framebuffer", "false")
.Param("mipmap_input", "false")
.Param("scale_type_x", "source")
.Param("scale_type_y", "source")
.Param("scale_x", "1.000000")
.Param("scale_y", "1.000000")
.Param("srgb_framebuffer", "false"));
	}
};
}
