/*
ShaderGlass preset deblur / deblur-luma imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/deblur/deblur-luma.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class DeblurDeblurLumaPresetDef : public PresetDef
{
public:
	DeblurDeblurLumaPresetDef() : PresetDef{}
	{
		Name = "deblur-luma";
		Category = "deblur";
	}

	virtual void Build() {
         	ShaderDefs.push_back(DeblurShadersDeblurLumaShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "viewport"));
	}
};
}
