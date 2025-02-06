/*
ShaderGlass preset misc / anti-flicker imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/misc/anti-flicker.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class MiscAntiFlickerPresetDef : public PresetDef
{
public:
	MiscAntiFlickerPresetDef() : PresetDef{}
	{
		Name = "anti-flicker";
		Category = "misc";
	}

	virtual void Build() {
         	ShaderDefs.push_back(MiscShadersAntiFlickerShaderDef()
.Param("scale_type", "source"));
	}
};
}
