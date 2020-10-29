/*
ShaderGlass preset cel / advcartoon imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/cel/advcartoon.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class CelAdvcartoonPresetDef : public PresetDef
{
public:
	CelAdvcartoonPresetDef() : PresetDef{}
	{
		Name = "advcartoon";
		Category = "cel";
	}

	virtual void Build() {
         	ShaderDefs.push_back(CelShadersAdvcartoonShaderDef()
.Param("filter_linear", "false"));
	}
};
}
