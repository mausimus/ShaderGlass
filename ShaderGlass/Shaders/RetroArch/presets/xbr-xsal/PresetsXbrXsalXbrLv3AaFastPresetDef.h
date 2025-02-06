/*
ShaderGlass preset presets-xbr-xsal / xbr-lv3-aa-fast imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/presets/xbr-xsal/xbr-lv3-aa-fast.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class PresetsXbrXsalXbrLv3AaFastPresetDef : public PresetDef
{
public:
	PresetsXbrXsalXbrLv3AaFastPresetDef() : PresetDef{}
	{
		Name = "xbr-lv3-aa-fast";
		Category = "presets-xbr-xsal";
	}

	virtual void Build() {
         	ShaderDefs.push_back(EdgeSmoothingXbrShadersSupportLinearizeShaderDef()
.Param("alias", "XbrSource")
.Param("filter_linear", "false")
.Param("scale", "1.000000")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(EdgeSmoothingXbrShadersXbrLv3MultipassXbrLv3Pass0ShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.000000")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(EdgeSmoothingXbrShadersXbrLv3MultipassXbrLv3Pass1ShaderDef()
.Param("filter_linear", "false")
.Param("scale", "5.000000")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(AntiAliasingShadersAaShader40ShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "viewport"));
	}
};
}
