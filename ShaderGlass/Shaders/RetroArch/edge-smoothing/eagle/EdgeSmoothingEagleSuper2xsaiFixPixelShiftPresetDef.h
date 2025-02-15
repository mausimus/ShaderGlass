/*
ShaderGlass preset edge-smoothing/eagle / super-2xsai-fix-pixel-shift imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/edge-smoothing/eagle/super-2xsai-fix-pixel-shift.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class EdgeSmoothingEagleSuper2xsaiFixPixelShiftPresetDef : public PresetDef
{
public:
	EdgeSmoothingEagleSuper2xsaiFixPixelShiftPresetDef() : PresetDef{}
	{
		Name = "super-2xsai-fix-pixel-shift";
		Category = "edge-smoothing/eagle";
	}

	virtual void Build() {
         	ShaderDefs.push_back(EdgeSmoothingEagleShadersSuper2xsaiShaderDef()
.Param("filter_linear", "false")
.Param("float_framebuffer", "false")
.Param("mipmap_input", "false")
.Param("scale_type_x", "source")
.Param("scale_type_y", "source")
.Param("scale_x", "2.000000")
.Param("scale_y", "2.000000")
.Param("srgb_framebuffer", "false")
.Param("wrap_mode", "clamp_to_border"));
         	ShaderDefs.push_back(EdgeSmoothingNnedi3ShadersJinc2CshiftRgbShaderDef()
.Param("filter_linear", "false")
.Param("float_framebuffer", "false")
.Param("mipmap_input", "false")
.Param("scale_type", "viewport")
.Param("srgb_framebuffer", "false")
.Param("wrap_mode", "clamp_to_border"));
            OverrideParam("JINC2_AR_STRENGTH", (float)0.000000);
            OverrideParam("JINC2_SINC", (float)0.880000);
            OverrideParam("JINC2_WINDOW_SINC", (float)0.500000);
	}
};
}
