/*
ShaderGlass preset crt / crt-potato-BVM imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/6f921ee4815a7894a33855974285b04545a4fa42/crt/crt-potato-BVM.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class CrtCrtPotatoBVMPresetDef : public PresetDef
{
public:
	CrtCrtPotatoBVMPresetDef() : PresetDef{}
	{
		Name = "crt-potato-BVM";
		Category = "crt";
	}

	virtual void Build() {
         	ShaderDefs.push_back(CrtShadersCrtPotatoShaderFilesUltra_potatoShaderDef()
.Param("alias", "PASS0")
.Param("filter_linear", "false")
.Param("float_framebuffer", "false")
.Param("mipmap_input", "false")
.Param("scale_type_x", "viewport")
.Param("scale_type_y", "viewport")
.Param("scale_x", "1.000000")
.Param("scale_y", "1.000000")
.Param("srgb_framebuffer", "false"));
            TextureDefs.push_back(CrtShadersCrtPotatoResourcesMask1TextureDef()
.Param("linear", "true")
.Param("mipmap", "false")
.Param("name", "MASK")
.Param("wrap_mode", "clamp_to_border"));
	}
};
}
