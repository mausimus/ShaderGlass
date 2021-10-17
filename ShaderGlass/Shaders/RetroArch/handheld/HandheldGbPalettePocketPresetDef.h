/*
ShaderGlass preset handheld / gb-palette-pocket imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/handheld/gb-palette-pocket.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class HandheldGbPalettePocketPresetDef : public PresetDef
{
public:
	HandheldGbPalettePocketPresetDef() : PresetDef{}
	{
		Name = "gb-palette-pocket";
		Category = "handheld";
	}

	virtual void Build() {
         	ShaderDefs.push_back(HandheldShadersGbPaletteGbPaletteShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "source"));
            TextureDefs.push_back(HandheldShadersGbPaletteResourcesPalettePocketTextureDef()
.Param("linear", "false")
.Param("name", "COLOR_PALETTE"));
	}
};
}
