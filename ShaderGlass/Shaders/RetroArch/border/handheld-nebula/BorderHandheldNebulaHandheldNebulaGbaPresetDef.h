/*
ShaderGlass preset border-handheld-nebula / handheld-nebula-gba imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/border/handheld-nebula/handheld-nebula-gba.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class BorderHandheldNebulaHandheldNebulaGbaPresetDef : public PresetDef
{
public:
	BorderHandheldNebulaHandheldNebulaGbaPresetDef() : PresetDef{}
	{
		Name = "handheld-nebula-gba";
		Category = "border-handheld-nebula";
	}

	virtual void Build() {
         	ShaderDefs.push_back(BorderShadersImgborderShaderDef()
.Param("scale_type_x", "absolute")
.Param("scale_type_y", "absolute")
.Param("scale_x", "280")
.Param("scale_y", "224"));
            TextureDefs.push_back(BorderHandheldNebulaHandheldNebulaGbaTextureDef()
.Param("name", "BORDER"));
	}
};
}
