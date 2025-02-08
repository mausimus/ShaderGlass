/*
ShaderGlass preset border-sgb / sgb-gbc-color-tvout+interlacing imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/border/sgb/sgb-gbc-color-tvout+interlacing.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class BorderSgbSgbGbcColorTvoutInterlacingPresetDef : public PresetDef
{
public:
	BorderSgbSgbGbcColorTvoutInterlacingPresetDef() : PresetDef{}
	{
		Name = "sgb-gbc-color-tvout+interlacing";
		Category = "border-sgb";
	}

	virtual void Build() {
         	ShaderDefs.push_back(HandheldShadersColorGbcColorShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(BorderShadersImgborderSgbShaderDef()
.Param("scale_type_x", "absolute")
.Param("scale_type_y", "absolute")
.Param("scale_x", "256")
.Param("scale_y", "224"));
         	ShaderDefs.push_back(CrtShadersTvoutTweaksShaderDef()
.Param("scale_type_x", "viewport")
.Param("scale_type_y", "source")
.Param("scale_x", "1.000000")
.Param("scale_y", "1.000000"));
         	ShaderDefs.push_back(MiscShadersImageAdjustmentShaderDef());
         	ShaderDefs.push_back(MiscShadersInterlacingShaderDef()
.Param("scale_type_y", "source")
.Param("scale_y", "2.000000"));
         	ShaderDefs.push_back(AutoBoxBoxCenterShaderDef()
.Param("filter_linear", "false"));
            TextureDefs.push_back(BorderSgbSgbTextureDef()
.Param("name", "BORDER"));
            OverrideParam("TVOUT_COMPOSITE_CONNECTION", (float)0.000000);
            OverrideParam("TVOUT_RESOLUTION", (float)320.000000);
            OverrideParam("TVOUT_TV_COLOR_LEVELS", (float)0.000000);
            OverrideParam("box_scale", (float)1.000000);
            OverrideParam("in_res_x", (float)160.000000);
            OverrideParam("in_res_y", (float)144.000000);
            OverrideParam("location", (float)0.500000);
	}
};
}
