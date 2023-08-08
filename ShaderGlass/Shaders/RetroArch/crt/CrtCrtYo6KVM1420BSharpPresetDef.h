/*
ShaderGlass preset crt / crt-yo6-KV-M1420B-sharp imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/6f921ee4815a7894a33855974285b04545a4fa42/crt/crt-yo6-KV-M1420B-sharp.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class CrtCrtYo6KVM1420BSharpPresetDef : public PresetDef
{
public:
	CrtCrtYo6KVM1420BSharpPresetDef() : PresetDef{}
	{
		Name = "crt-yo6-KV-M1420B-sharp";
		Category = "crt";
	}

	virtual void Build() {
         	ShaderDefs.push_back(CrtShadersCrtYo6CrtYo6NativeResolutionShaderDef()
.Param("filter_linear", "true")
.Param("scale_type", "absolute")
.Param("scale_x", "439")
.Param("scale_y", "233"));
         	ShaderDefs.push_back(CrtShadersCrtYo6CrtYo6WarpShaderDef()
.Param("filter_linear", "true")
.Param("scale_type", "absolute")
.Param("scale_x", "1684")
.Param("scale_y", "1329"));
         	ShaderDefs.push_back(WindowedShadersJinc2ShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "viewport"));
            TextureDefs.push_back(CrtShadersCrtYo6KVM1420BTextureDef()
.Param("linear", "false")
.Param("name", "TEX_CRT"));
	}
};
}
