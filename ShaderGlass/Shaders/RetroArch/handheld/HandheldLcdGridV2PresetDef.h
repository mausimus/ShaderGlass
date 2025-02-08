/*
ShaderGlass preset handheld / lcd-grid-v2 imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/handheld/lcd-grid-v2.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class HandheldLcdGridV2PresetDef : public PresetDef
{
public:
	HandheldLcdGridV2PresetDef() : PresetDef{}
	{
		Name = "lcd-grid-v2";
		Category = "handheld";
	}

	virtual void Build() {
         	ShaderDefs.push_back(HandheldShadersLcdCgwgLcdGridV2ShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "viewport"));
            OverrideParam("BGR", (float)0.000000);
            OverrideParam("BSUBPIX_B", (float)0.750000);
            OverrideParam("BSUBPIX_G", (float)0.000000);
            OverrideParam("BSUBPIX_R", (float)0.000000);
            OverrideParam("GSUBPIX_B", (float)0.000000);
            OverrideParam("GSUBPIX_G", (float)0.750000);
            OverrideParam("GSUBPIX_R", (float)0.000000);
            OverrideParam("RSUBPIX_B", (float)0.000000);
            OverrideParam("RSUBPIX_G", (float)0.000000);
            OverrideParam("RSUBPIX_R", (float)0.750000);
            OverrideParam("ambient", (float)0.000000);
            OverrideParam("blacklevel", (float)0.000000);
            OverrideParam("gain", (float)1.500000);
            OverrideParam("gamma", (float)2.200000);
	}
};
}
