/*
ShaderGlass preset ntsc / ntsc-simple imported from RetroArch:
https://github.com/libretro/slang-shaders/blob/23046258f7fd02242cc6dd4c08c997a8ddb84935/ntsc/ntsc-simple.slangp
See original file for credits and usage license. 
This file is auto-generated, do not modify directly.
*/

#pragma once

namespace RetroArch
{
class NtscNtscSimplePresetDef : public PresetDef
{
public:
	NtscNtscSimplePresetDef() : PresetDef{}
	{
		Name = "ntsc-simple";
		Category = "ntsc";
	}

	virtual void Build() {
         	ShaderDefs.push_back(StockStockShaderDef()
.Param("scale_type_x", "source")
.Param("scale_type_y", "source")
.Param("scale_x", "4.000000")
.Param("scale_y", "1.000000"));
         	ShaderDefs.push_back(NtscShadersNtscSimpleNtscSimple1ShaderDef()
.Param("scale_type_x", "source")
.Param("scale_type_y", "source")
.Param("scale_x", "1.000000")
.Param("scale_y", "1.000000"));
         	ShaderDefs.push_back(NtscShadersNtscSimpleNtscSimple2ShaderDef()
.Param("scale_type_x", "source")
.Param("scale_type_y", "source")
.Param("scale_x", "0.500000")
.Param("scale_y", "1.000000"));
         	ShaderDefs.push_back(NtscShadersNtscSimpleKaizerLpSmallShaderDef()
.Param("scale_type_x", "source")
.Param("scale_type_y", "source")
.Param("scale_x", "1.000000")
.Param("scale_y", "1.000000"));
	}
};
}
