/*
ShaderGlass preset border-sgb / sgb-crt-geom-1x imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/border/sgb/sgb-crt-geom-1x.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class BorderSgbSgbCrtGeom1xPresetDef : public PresetDef
{
public:
	BorderSgbSgbCrtGeom1xPresetDef() : PresetDef{}
	{
		Name = "sgb-crt-geom-1x";
		Category = "border-sgb";
	}

	virtual void Build() {
         	ShaderDefs.push_back(BorderShadersImgborderSgbShaderDef()
.Param("scale_type_x", "absolute")
.Param("scale_type_y", "absolute")
.Param("scale_x", "256")
.Param("scale_y", "224"));
         	ShaderDefs.push_back(CrtShadersCrtGeomShaderDef());
            TextureDefs.push_back(BorderSgbSgbTextureDef()
.Param("linear", "true")
.Param("name", "BORDER"));
            OverrideParam("border_on_top", (float)0.000000);
            OverrideParam("box_scale", (float)1.000000);
            OverrideParam("in_res_x", (float)160.000000);
            OverrideParam("in_res_y", (float)144.000000);
	}
};
}
