/*
ShaderGlass preset eagle / 2xsai-fix-pixel-shift imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/6f921ee4815a7894a33855974285b04545a4fa42/eagle/2xsai-fix-pixel-shift.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class Eagle2xsaiFixPixelShiftPresetDef : public PresetDef
{
public:
	Eagle2xsaiFixPixelShiftPresetDef() : PresetDef{}
	{
		Name = "2xsai-fix-pixel-shift";
		Category = "eagle";
	}

	virtual void Build() {
         	ShaderDefs.push_back(EagleShaders2xsaiShaderDef()
.Param("filter_linear", "false")
.Param("float_framebuffer", "false")
.Param("mipmap_input", "false")
.Param("scale_type_x", "source")
.Param("scale_type_y", "source")
.Param("scale_x", "2.000000")
.Param("scale_y", "2.000000")
.Param("srgb_framebuffer", "false"));
         	ShaderDefs.push_back(Nnedi3ShadersJinc2CshiftRgbShaderDef()
.Param("filter_linear", "false")
.Param("float_framebuffer", "false")
.Param("mipmap_input", "false")
.Param("scale_type", "viewport")
.Param("srgb_framebuffer", "false"));
	}
};
}
