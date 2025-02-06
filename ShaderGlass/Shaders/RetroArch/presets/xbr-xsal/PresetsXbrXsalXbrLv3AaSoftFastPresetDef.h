/*
ShaderGlass preset presets-xbr-xsal / xbr-lv3-aa-soft-fast imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/presets/xbr-xsal/xbr-lv3-aa-soft-fast.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class PresetsXbrXsalXbrLv3AaSoftFastPresetDef : public PresetDef
{
public:
	PresetsXbrXsalXbrLv3AaSoftFastPresetDef() : PresetDef{}
	{
		Name = "xbr-lv3-aa-soft-fast";
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
.Param("scale", "4.000000")
.Param("scale_type", "source"));
         	ShaderDefs.push_back(AntiAliasingShadersAaShader40ShaderDef()
.Param("filter_linear", "false")
.Param("scale", "1.0")
.Param("scale_type", "viewport"));
	}
};
}
