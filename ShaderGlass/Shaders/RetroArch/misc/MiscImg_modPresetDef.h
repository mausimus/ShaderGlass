/*
ShaderGlass preset misc / img_mod imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/misc/img_mod.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class MiscImg_modPresetDef : public PresetDef
{
public:
	MiscImg_modPresetDef() : PresetDef{}
	{
		Name = "img_mod";
		Category = "misc";
	}

	virtual void Build() {
         	ShaderDefs.push_back(MiscShadersImg_modShaderDef()
.Param("scale_type", "viewport"));
            TextureDefs.push_back(ReshadeShadersLUT16TextureDef()
.Param("name", "SamplerLUT1"));
            TextureDefs.push_back(ReshadeShadersLUT16TextureDef()
.Param("name", "SamplerLUT2"));
	}
};
}
