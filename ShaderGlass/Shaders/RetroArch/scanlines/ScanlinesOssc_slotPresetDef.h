/*
ShaderGlass preset scanlines / ossc_slot imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/scanlines/ossc_slot.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class ScanlinesOssc_slotPresetDef : public PresetDef
{
public:
	ScanlinesOssc_slotPresetDef() : PresetDef{}
	{
		Name = "ossc_slot";
		Category = "scanlines";
	}

	virtual void Build() {
         	ShaderDefs.push_back(MiscShadersSimple_color_controlsShaderDef()
.Param("float_framebuffer", "false")
.Param("mipmap_input", "false")
.Param("scale_type_x", "source")
.Param("scale_type_y", "source")
.Param("scale_x", "1.000000")
.Param("scale_y", "1.000000")
.Param("srgb_framebuffer", "false")
.Param("wrap_mode", "clamp_to_border"));
         	ShaderDefs.push_back(InterpolationShadersLanczos3XShaderDef()
.Param("filter_linear", "false")
.Param("float_framebuffer", "false")
.Param("mipmap_input", "false")
.Param("scale_type_x", "viewport")
.Param("scale_type_y", "source")
.Param("scale_x", "1.000000")
.Param("scale_y", "1.000000")
.Param("srgb_framebuffer", "false")
.Param("wrap_mode", "clamp_to_edge"));
         	ShaderDefs.push_back(InterpolationShadersLanczos3YShaderDef()
.Param("filter_linear", "false")
.Param("float_framebuffer", "false")
.Param("mipmap_input", "false")
.Param("srgb_framebuffer", "false")
.Param("wrap_mode", "clamp_to_edge"));
         	ShaderDefs.push_back(ScanlinesShadersOsscShaderDef()
.Param("float_framebuffer", "false")
.Param("mipmap_input", "false")
.Param("scale_type_x", "viewport")
.Param("scale_type_y", "viewport")
.Param("scale_x", "1.000000")
.Param("scale_y", "1.000000")
.Param("srgb_framebuffer", "false")
.Param("wrap_mode", "clamp_to_border"));
         	ShaderDefs.push_back(CrtShadersSlotmaskShaderDef()
.Param("float_framebuffer", "false")
.Param("mipmap_input", "false")
.Param("srgb_framebuffer", "false")
.Param("wrap_mode", "clamp_to_border"));
            OverrideParam("BLACK", (float)0.010000);
            OverrideParam("BRIGHTNESS", (float)1.050000);
            OverrideParam("SAT", (float)1.100000);
            OverrideParam("contrast", (float)1.040000);
            OverrideParam("feedback_pass", (float)0.000000);
            OverrideParam("gamma_out_blue", (float)2.500000);
            OverrideParam("gamma_out_red", (float)2.400000);
            OverrideParam("postbr", (float)1.150000);
	}
};
}
