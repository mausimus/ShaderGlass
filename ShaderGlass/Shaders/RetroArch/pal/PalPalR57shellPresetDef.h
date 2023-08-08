/*
ShaderGlass preset pal / pal-r57shell imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/6f921ee4815a7894a33855974285b04545a4fa42/pal/pal-r57shell.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class PalPalR57shellPresetDef : public PresetDef
{
public:
	PalPalR57shellPresetDef() : PresetDef{}
	{
		Name = "pal-r57shell";
		Category = "pal";
	}

	virtual void Build() {
         	ShaderDefs.push_back(PalShadersPalR57shellShaderDef()
.Param("alias", "ORIG_LINEARIZED")
.Param("filter_linear", "false")
.Param("float_framebuffer", "false")
.Param("mipmap_input", "false")
.Param("scale_type_x", "viewport")
.Param("scale_type_y", "source")
.Param("scale_x", "1.000000")
.Param("scale_y", "1.000000")
.Param("srgb_framebuffer", "false"));
            TextureDefs.push_back(PalResourcesNes_lutTextureDef()
.Param("linear", "false")
.Param("mipmap", "false")
.Param("name", "nes_lut")
.Param("wrap_mode", "repeat"));
	}
};
}
