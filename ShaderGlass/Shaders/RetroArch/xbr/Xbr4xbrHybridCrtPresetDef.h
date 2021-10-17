/*
ShaderGlass preset xbr / 4xbr-hybrid-crt imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/master/xbr/4xbr-hybrid-crt.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class Xbr4xbrHybridCrtPresetDef : public PresetDef
{
public:
	Xbr4xbrHybridCrtPresetDef() : PresetDef{}
	{
		Name = "4xbr-hybrid-crt";
		Category = "xbr";
	}

	virtual void Build() {
         	ShaderDefs.push_back(XbrShaders4xbrHybridCrtShaderDef()
.Param("filter_linear", "false"));
	}
};
}
