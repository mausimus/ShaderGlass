/*
ShaderGlass preset presets/crt-plus-signal / my_old_tv imported from RetroArch:
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
		Category = "presets/crt-plus-signal";
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
.Param("srgb_framebuffer", "false")
.Param("wrap_mode", "clamp_to_border"));
            OverrideParam("Shadowmask", (float)1.000000);
            OverrideParam("WP", (float)75.000000);
            OverrideParam("beamhigh", (float)1.200000);
            OverrideParam("blurx", (float)0.850000);
            OverrideParam("blury", (float)0.050000);
            OverrideParam("br", (float)0.155000);
            OverrideParam("brightboost1", (float)1.450000);
            OverrideParam("contrast", (float)1.050000);
            OverrideParam("gb", (float)0.055000);
            OverrideParam("glow", (float)2.000000);
            OverrideParam("glow_str", (float)0.000100);
            OverrideParam("nois", (float)10.000000);
            OverrideParam("palette_fix", (float)1.000000);
            OverrideParam("postbr", (float)1.040000);
            OverrideParam("preserve", (float)0.700000);
            OverrideParam("quality", (float)0.700000);
            OverrideParam("rb", (float)0.070000);
            OverrideParam("sat", (float)1.100000);
            OverrideParam("vignette", (float)1.000000);
            OverrideParam("vpower", (float)0.120000);
            OverrideParam("vstr", (float)48.000000);
	}
};
}
