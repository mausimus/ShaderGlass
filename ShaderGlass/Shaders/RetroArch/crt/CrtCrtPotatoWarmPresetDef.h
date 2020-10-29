/*
ShaderGlass preset crt / crt-potato-warm imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/crt/crt-potato-warm.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class CrtCrtPotatoWarmPresetDef : public PresetDef
{
public:
	CrtCrtPotatoWarmPresetDef() : PresetDef{}
	{
		Name = "crt-potato-warm";
		Category = "crt";
	}

	virtual void Build() {
         	ShaderDefs.push_back(CrtShadersCrtPotatoShaderFilesCrtPotatoShaderDef()
.Param("alias", "PASS0")
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "viewport"));
            TextureDefs.push_back(CrtShadersCrtPotatoResourcesCrtPotatoThickTextureDef()
.Param("linear", "false")
.Param("name", "MASK")
.Param("wrap_mode", "repeat"));
	}
};
}
