/*
ShaderGlass preset crt / crt-yo6-KV-21CL10B imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/crt/crt-yo6-KV-21CL10B.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class CrtCrtYo6KV21CL10BPresetDef : public PresetDef
{
public:
	CrtCrtYo6KV21CL10BPresetDef() : PresetDef{}
	{
		Name = "crt-yo6-KV-21CL10B";
		Category = "crt";
	}

	virtual void Build() {
         	ShaderDefs.push_back(CrtShadersCrtYo6CrtYo6FlatTrinitronTvShaderDef()
.Param("filter_linear", "true")
.Param("scale_type", "viewport"));
            TextureDefs.push_back(CrtShadersCrtYo6PhosphorFlatTrinitronTvTextureDef()
.Param("linear", "false")
.Param("name", "TEX"));
	}
};
}
