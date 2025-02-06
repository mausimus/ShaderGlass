/*
ShaderGlass preset border-sgb / sgb-gbc-color imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/border/sgb/sgb-gbc-color.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class BorderSgbSgbGbcColorPresetDef : public PresetDef
{
public:
	BorderSgbSgbGbcColorPresetDef() : PresetDef{}
	{
		Name = "sgb-gbc-color";
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
            TextureDefs.push_back(BorderSgbSgbTextureDef()
.Param("name", "BORDER"));
	}
};
}
