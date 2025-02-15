/*
ShaderGlass preset border/handheld-nebula / handheld-nebula-gb+dot imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/border/handheld-nebula/handheld-nebula-gb+dot.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class BorderHandheldNebulaHandheldNebulaGbDotPresetDef : public PresetDef
{
public:
	BorderHandheldNebulaHandheldNebulaGbDotPresetDef() : PresetDef{}
	{
		Name = "handheld-nebula-gb+dot";
		Category = "border/handheld-nebula";
	}

	virtual void Build() {
         	ShaderDefs.push_back(BorderShadersImgborderShaderDef()
.Param("scale_type_x", "absolute")
.Param("scale_type_y", "absolute")
.Param("scale_x", "280")
.Param("scale_y", "224"));
         	ShaderDefs.push_back(HandheldShadersDotShaderDef());
            TextureDefs.push_back(BorderHandheldNebulaHandheldNebulaGbTextureDef()
.Param("name", "BORDER"));
            OverrideParam("box_scale", (float)1.000000);
            OverrideParam("in_res_x", (float)160.000000);
            OverrideParam("in_res_y", (float)144.000000);
            OverrideParam("location", (float)0.500000);
	}
};
}
