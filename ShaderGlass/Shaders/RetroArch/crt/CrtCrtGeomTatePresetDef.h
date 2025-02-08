/*
ShaderGlass preset crt / crt-geom-tate imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/crt/crt-geom-tate.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class CrtCrtGeomTatePresetDef : public PresetDef
{
public:
	CrtCrtGeomTatePresetDef() : PresetDef{}
	{
		Name = "crt-geom-tate";
		Category = "crt";
	}

	virtual void Build() {
         	ShaderDefs.push_back(CrtShadersCrtGeomShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "viewport"));
            OverrideParam("CRTgamma", (float)2.200000);
            OverrideParam("CURVATURE", (float)1.000000);
            OverrideParam("DOTMASK", (float)0.000000);
            OverrideParam("R", (float)1.800000);
            OverrideParam("cornersize", (float)0.015500);
            OverrideParam("d", (float)1.500000);
            OverrideParam("interlace_detect", (float)0.000000);
            OverrideParam("invert_aspect", (float)0.000000);
            OverrideParam("lum", (float)0.070000);
            OverrideParam("scanline_weight", (float)0.300000);
            OverrideParam("vertical_scanlines", (float)1.000000);
            OverrideParam("y_tilt", (float)-0.150000);
	}
};
}
