/*
ShaderGlass preset bezel-uborder / ambient-light-crt imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/bezel/uborder/ambient-light-crt.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class BezelUborderAmbientLightCrtPresetDef : public PresetDef
{
public:
	BezelUborderAmbientLightCrtPresetDef() : PresetDef{}
	{
		Name = "ambient-light-crt";
		Category = "bezel-uborder";
	}

	virtual void Build() {
         	ShaderDefs.push_back(BezelUborderShadersSupport_shadersFeedbackShaderDef()
.Param("filter_linear", "true")
.Param("scale", "1.000000")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(BezelUborderShadersSupport_shadersAmbientLightShaderDef()
.Param("alias", "BORDER")
.Param("filter_linear", "true")
.Param("mipmap_input", "true")
.Param("scale", "4.000000")
.Param("scale_type", "source")
.Param("wrap_mode", "clamp_to_border"));
         	ShaderDefs.push_back(BezelUborderShadersSupport_shadersOriginalShaderDef()
.Param("alias", "OriginalFake")
.Param("filter_linear", "false")
.Param("float_framebuffer", "false")
.Param("mipmap_input", "false")
.Param("scale_type_x", "source")
.Param("scale_type_y", "source")
.Param("scale_x", "0.250000")
.Param("scale_y", "0.250000")
.Param("srgb_framebuffer", "false")
.Param("wrap_mode", "clamp_to_border"));
         	ShaderDefs.push_back(BezelUborderShadersContent_shadersCrtNobodyShaderDef()
.Param("alias", "CRTPass")
.Param("filter_linear", "false")
.Param("float_framebuffer", "false")
.Param("mipmap_input", "false")
.Param("scale_type_x", "viewport")
.Param("scale_type_y", "viewport")
.Param("scale_x", "1.000000")
.Param("scale_y", "1.000000")
.Param("srgb_framebuffer", "false")
.Param("wrap_mode", "clamp_to_border"));
         	ShaderDefs.push_back(BezelUborderShadersUborderShaderDef()
.Param("filter_linear", "true")
.Param("float_framebuffer", "false")
.Param("mipmap_input", "false")
.Param("scale_type_x", "viewport")
.Param("scale_type_y", "viewport")
.Param("scale_x", "1.000000")
.Param("scale_y", "1.000000")
.Param("srgb_framebuffer", "false")
.Param("wrap_mode", "clamp_to_border"));
            OverrideParam("AG_effect", (float)1.300000);
            OverrideParam("feedback_pass", (float)0.000000);
            OverrideParam("geom_R", (float)4.000000);
            OverrideParam("geom_cornersize", (float)0.016000);
            OverrideParam("geom_cornersmooth", (float)280.000000);
            OverrideParam("geom_curvature", (float)1.000000);
            OverrideParam("geom_d", (float)1.000000);
            OverrideParam("mixfactor", (float)0.950000);
            OverrideParam("ub_border_top", (float)0.000000);
	}
};
}
