/*
ShaderGlass preset border / autocrop-koko imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/border/autocrop-koko.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class BorderAutocropKokoPresetDef : public PresetDef
{
public:
	BorderAutocropKokoPresetDef() : PresetDef{}
	{
		Name = "autocrop-koko";
		Category = "border";
	}

	virtual void Build() {
         	ShaderDefs.push_back(BorderShadersAutocropKokoAutocrop0_precutShaderDef()
.Param("alias", "autocrop_precut")
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "source")
.Param("wrap_mode", "clamp_to_border"));
         	ShaderDefs.push_back(BorderShadersAutocropKokoAutocrop1_computeShaderDef()
.Param("alias", "autocrop_compute")
.Param("filter_linear", "false")
.Param("float_framebuffer", "true")
.Param("mipmap_input", "true")
.Param("scale", "1.0")
.Param("scale_type", "source")
.Param("wrap_mode", "clamp_to_border"));
         	ShaderDefs.push_back(BorderShadersAutocropKokoAutocrop2_displayShaderDef()
.Param("alias", "autocrop_display")
.Param("filter_linear", "true")
.Param("scale_type", "viewport"));
	}
};
}
