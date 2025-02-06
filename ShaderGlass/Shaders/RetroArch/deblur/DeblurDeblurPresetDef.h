/*
ShaderGlass preset deblur / deblur imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/deblur/deblur.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class DeblurDeblurPresetDef : public PresetDef
{
public:
	DeblurDeblurPresetDef() : PresetDef{}
	{
		Name = "deblur";
		Category = "deblur";
	}

	virtual void Build() {
         	ShaderDefs.push_back(DeblurShadersDeblurShaderDef()
.Param("filter_linear", "false")
.Param("scale_type", "viewport"));
	}
};
}
