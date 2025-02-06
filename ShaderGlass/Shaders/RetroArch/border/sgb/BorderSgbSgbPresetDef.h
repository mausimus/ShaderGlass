/*
ShaderGlass preset border-sgb / sgb imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/border/sgb/sgb.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class BorderSgbSgbPresetDef : public PresetDef
{
public:
	BorderSgbSgbPresetDef() : PresetDef{}
	{
		Name = "sgb";
		Category = "border-sgb";
	}

	virtual void Build() {
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
