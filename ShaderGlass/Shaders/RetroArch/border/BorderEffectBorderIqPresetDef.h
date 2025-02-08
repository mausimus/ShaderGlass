/*
ShaderGlass preset border / effect-border-iq imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/border/effect-border-iq.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class BorderEffectBorderIqPresetDef : public PresetDef
{
public:
	BorderEffectBorderIqPresetDef() : PresetDef{}
	{
		Name = "effect-border-iq";
		Category = "border";
	}

	virtual void Build() {
         	ShaderDefs.push_back(BorderShadersEffectBorderIqShaderDef()
.Param("alias", "BORDERS")
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "viewport"));
            TextureDefs.push_back(BorderTexturesTex11TextureDef()
.Param("linear", "true")
.Param("mipmap", "true")
.Param("name", "iChannel0")
.Param("wrap_mode", "repeat"));
            OverrideParam("framecount_mod0", (float)10000.000000);
	}
};
}
