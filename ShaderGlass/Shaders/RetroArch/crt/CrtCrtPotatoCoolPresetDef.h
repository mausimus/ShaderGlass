/*
ShaderGlass preset crt / crt-potato-cool imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/6f921ee4815a7894a33855974285b04545a4fa42/crt/crt-potato-cool.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class CrtCrtPotatoCoolPresetDef : public PresetDef
{
public:
	CrtCrtPotatoCoolPresetDef() : PresetDef{}
	{
		Name = "crt-potato-cool";
		Category = "crt";
	}

	virtual void Build() {
         	ShaderDefs.push_back(CrtShadersCrtPotatoShaderFilesCrtPotatoShaderDef()
.Param("alias", "PASS0")
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "viewport"));
            TextureDefs.push_back(CrtShadersCrtPotatoResourcesCrtPotatoThinTextureDef()
.Param("linear", "false")
.Param("name", "MASK")
.Param("wrap_mode", "repeat"));
	}
};
}
