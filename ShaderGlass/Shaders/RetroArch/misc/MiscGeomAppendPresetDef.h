/*
ShaderGlass preset misc / geom-append imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/misc/geom-append.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class MiscGeomAppendPresetDef : public PresetDef
{
public:
	MiscGeomAppendPresetDef() : PresetDef{}
	{
		Name = "geom-append";
		Category = "misc";
	}

	virtual void Build() {
         	ShaderDefs.push_back(MiscShadersGeomShaderDef()
.Param("filter_linear", "true")
.Param("mipmap_input", "true")
.Param("scale_type", "viewport")
.Param("wrap_mode", "clamp_to_border"));
            OverrideParam("geom_lod_bias", (float)0.500000);
	}
};
}
