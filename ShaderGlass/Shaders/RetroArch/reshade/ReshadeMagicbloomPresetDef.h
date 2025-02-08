/*
ShaderGlass preset reshade / magicbloom imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/reshade/magicbloom.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class ReshadeMagicbloomPresetDef : public PresetDef
{
public:
	ReshadeMagicbloomPresetDef() : PresetDef{}
	{
		Name = "magicbloom";
		Category = "reshade";
	}

	virtual void Build() {
         	ShaderDefs.push_back(StockStockShaderDef()
.Param("alias", "PreBloomPass")
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "viewport")
.Param("wrap_mode", "mirrored_repeat"));
         	ShaderDefs.push_back(ReshadeShadersMagicbloomMagicbloom_blur_pass_1ShaderDef()
.Param("alias", "tMagicBloom_1")
.Param("filter_linear", "true")
.Param("mipmap_input", "true")
.Param("scale_type", "absolute")
.Param("scale_x", "1920")
.Param("scale_y", "1080")
.Param("wrap_mode", "mirrored_repeat"));
         	ShaderDefs.push_back(ReshadeShadersMagicbloomMagicbloom_blur_pass_2ShaderDef()
.Param("alias", "tMagicBloom_2")
.Param("filter_linear", "true")
.Param("mipmap_input", "true")
.Param("scale", "0.75")
.Param("scale_type", "source")
.Param("wrap_mode", "mirrored_repeat"));
         	ShaderDefs.push_back(ReshadeShadersMagicbloomMagicbloom_blur_pass_3ShaderDef()
.Param("alias", "tMagicBloom_3")
.Param("filter_linear", "true")
.Param("mipmap_input", "true")
.Param("scale", "0.75")
.Param("scale_type", "source")
.Param("wrap_mode", "mirrored_repeat"));
         	ShaderDefs.push_back(ReshadeShadersMagicbloomMagicbloom_blur_pass_4ShaderDef()
.Param("alias", "tMagicBloom_4")
.Param("filter_linear", "true")
.Param("mipmap_input", "true")
.Param("scale", "0.75")
.Param("scale_type", "source")
.Param("wrap_mode", "mirrored_repeat"));
         	ShaderDefs.push_back(ReshadeShadersMagicbloomMagicbloom_blur_pass_5ShaderDef()
.Param("alias", "tMagicBloom_5")
.Param("filter_linear", "true")
.Param("mipmap_input", "true")
.Param("scale", "0.75")
.Param("scale_type", "source")
.Param("wrap_mode", "mirrored_repeat"));
         	ShaderDefs.push_back(ReshadeShadersMagicbloomMagicbloom_blur_pass_6ShaderDef()
.Param("alias", "tMagicBloom_6")
.Param("filter_linear", "true")
.Param("mipmap_input", "true")
.Param("scale", "0.5")
.Param("scale_type", "source")
.Param("wrap_mode", "mirrored_repeat"));
         	ShaderDefs.push_back(ReshadeShadersMagicbloomMagicbloom_blur_pass_7ShaderDef()
.Param("alias", "tMagicBloom_7")
.Param("filter_linear", "true")
.Param("mipmap_input", "true")
.Param("scale", "0.5")
.Param("scale_type", "source")
.Param("wrap_mode", "mirrored_repeat"));
         	ShaderDefs.push_back(ReshadeShadersMagicbloomMagicbloom_blur_pass_8ShaderDef()
.Param("alias", "tMagicBloom_8")
.Param("filter_linear", "true")
.Param("mipmap_input", "true")
.Param("scale", "0.5")
.Param("scale_type", "source")
.Param("wrap_mode", "mirrored_repeat"));
         	ShaderDefs.push_back(ReshadeShadersMagicbloomMagicbloom_get_small_lumaShaderDef()
.Param("alias", "tMagicBloom_Small")
.Param("filter_linear", "true")
.Param("mipmap_input", "true")
.Param("scale_type", "absolute")
.Param("scale_x", "256")
.Param("scale_y", "256"));
         	ShaderDefs.push_back(ReshadeShadersMagicbloomMagicbloom_get_adaptShaderDef()
.Param("alias", "tMagicBloom_Adapt")
.Param("filter_linear", "true")
.Param("mipmap_input", "true")
.Param("scale", "1.0")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(ReshadeShadersMagicbloomMagicbloom_blendShaderDef()
.Param("filter_linear", "true")
.Param("mipmap_input", "true")
.Param("scale", "1.0")
.Param("scale_type", "viewport"));
            TextureDefs.push_back(ReshadeShadersMagicbloomMagicBloom_DirtTextureDef()
.Param("name", "tMagicBloom_Dirt"));
	}
};
}
