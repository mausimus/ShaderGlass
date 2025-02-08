/*
ShaderGlass preset presets / crt-geom-simple imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/presets/crt-geom-simple.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class PresetsCrtGeomSimplePresetDef : public PresetDef
{
public:
	PresetsCrtGeomSimplePresetDef() : PresetDef{}
	{
		Name = "crt-geom-simple";
		Category = "presets";
	}

	virtual void Build() {
         	ShaderDefs.push_back(MiscShadersSimple_color_controlsShaderDef()
.Param("filter_linear", "false")
.Param("scale_type_x", "source")
.Param("scale_type_y", "source")
.Param("scale_x", "1.000000")
.Param("scale_y", "1.000000"));
         	ShaderDefs.push_back(MiscShadersChromaticityShaderDef()
.Param("filter_linear", "false"));
         	ShaderDefs.push_back(CrtShadersCrtSimpleShaderDef()
.Param("filter_linear", "true")
.Param("scale_type_x", "viewport")
.Param("scale_type_y", "viewport")
.Param("scale_x", "1.000000")
.Param("scale_y", "1.000000"));
            OverrideParam("BLACK", (float)0.010000);
            OverrideParam("BRIGHTNESS", (float)1.020000);
            OverrideParam("DISTORTION", (float)0.100000);
            OverrideParam("INPUTGAMMA", (float)2.000000);
            OverrideParam("OUTPUTGAMMA", (float)2.000000);
            OverrideParam("SAT", (float)1.080000);
            OverrideParam("TEMP", (float)7079.000000);
            OverrideParam("contrast", (float)1.020000);
            OverrideParam("feedback_pass", (float)0.000000);
            OverrideParam("gamma_out_green", (float)2.100000);
            OverrideParam("gamma_out_red", (float)2.300000);
            OverrideParam("postbr", (float)1.050000);
	}
};
}
